/*
 * Copyright (c) 2008-2015, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// Created by sancar koyunlu on 6/3/13.



#include "hazelcast/client/spi/PartitionService.h"
#include "hazelcast/client/spi/ClusterService.h"
#include "hazelcast/client/spi/LifecycleService.h"
#include "hazelcast/client/spi/InvocationService.h"

#include "hazelcast/client/serialization/pimpl/SerializationService.h"
#include "hazelcast/client/spi/ClientContext.h"
#include "hazelcast/client/exception/IllegalStateException.h"
#include "hazelcast/client/connection/CallFuture.h"
#include "hazelcast/client/protocol/codec/ClientGetPartitionsCodec.h"

#include <climits>

#include <vector>

namespace hazelcast {
    namespace client {
        namespace spi {
            PartitionService::PartitionService(spi::ClientContext& clientContext)
            : clientContext(clientContext)
            , updating(false)
            , partitionCount(0) {

            }

            bool PartitionService::start() {
                util::LockGuard lg(startLock);
                if (!getInitialPartitions()) {
                    return false;
                }
                util::Thread *partitionListener = new util::Thread("hz.partitionListener", PartitionService::staticRunListener, this);
                partitionListenerThread.reset(partitionListener);
                return true;
            }

            void PartitionService::shutdown() {
                util::LockGuard lg(startLock);
                if (partitionListenerThread.get() != NULL) {
                    partitionListenerThread->interrupt();
                    partitionListenerThread->join();
                }
            }

            boost::shared_ptr<Address> PartitionService::getPartitionOwner(int partitionId) {
                return partitions.get(partitionId);
            }

            int PartitionService::getPartitionId(const serialization::pimpl::Data& key) {
                const int pc = partitionCount;
                int hash = key.getPartitionHash();
                return (hash == INT_MIN) ? 0 : abs(hash) % pc;
            }

            void PartitionService::staticRunListener(util::ThreadArgs& args) {
                PartitionService *partitionService = (PartitionService *)args.arg0;
                partitionService->runListener(args.currentThread);
            }

            void PartitionService::runListener(util::Thread *currentThread) {
                while (clientContext.getLifecycleService().isRunning()) {
                    try {
                        currentThread->interruptibleSleep(10);
                        if (!clientContext.getLifecycleService().isRunning()) {
                            break;
                        }
                        runRefresher();
                    } catch (exception::IException& e) {
                        util::ILogger::getLogger().warning(std::string("PartitionService::runListener") + e.what());
                    }
                }
            }

            void PartitionService::runRefresher() {

                if (updating.compareAndSet(false, true)) {
                    try {
                        std::auto_ptr<protocol::ClientMessage> partitionResponse;
                        std::auto_ptr<Address> ptr = clientContext.getClusterService().getMasterAddress();
                        if (ptr.get() == NULL) {
                            partitionResponse = getPartitionsFrom();
                        } else {
                            partitionResponse = getPartitionsFrom(*ptr.get());
                        }
                        if (partitionResponse.get() != NULL) {
                            processPartitionResponse(*partitionResponse);
                        }
                    } catch (hazelcast::client::exception::IException& e) {
                        util::ILogger::getLogger().finest(std::string("Exception partitionService::runRefresher ") + e.what());
                    } catch (...) {
                        util::ILogger::getLogger().finest(std::string("Unkown exception partitionService::runRefresher "));
                        throw;
                    }
                    updating = false;
                }

            }

            std::auto_ptr<protocol::ClientMessage> PartitionService::getPartitionsFrom(const Address& address) {
                std::auto_ptr<protocol::ClientMessage> responseMessage;
                try {
                    std::auto_ptr<protocol::ClientMessage> requestMessage = protocol::codec::ClientGetPartitionsCodec::RequestParameters::encode();

                    connection::CallFuture future = clientContext.getInvocationService().invokeOnTarget(
                            requestMessage, address);

                    responseMessage = future.get();

                } catch (exception::IOException& e) {
                    util::ILogger::getLogger().severe(std::string("Error while fetching cluster partition table => ") + e.what());
                }
                return responseMessage;
            }

            std::auto_ptr<protocol::ClientMessage> PartitionService::getPartitionsFrom() {
                std::auto_ptr<protocol::ClientMessage> responseMessage;
                try {
                    std::auto_ptr<protocol::ClientMessage> requestMessage = protocol::codec::ClientGetPartitionsCodec::RequestParameters::encode();

                    connection::CallFuture future = clientContext.getInvocationService().invokeOnRandomTarget(
                            requestMessage);

                    responseMessage = future.get();

                } catch (exception::IOException& e) {
                    util::ILogger::getLogger().severe(std::string("Error while fetching cluster partition table => ") + e.what());
                }
                return responseMessage;
            }

            bool PartitionService::processPartitionResponse(protocol::ClientMessage &response) {
                protocol::codec::ClientGetPartitionsCodec::ResponseParameters result =
                        protocol::codec::ClientGetPartitionsCodec::ResponseParameters::decode(response);

                partitions.clear();
                for (std::vector<std::pair<Address, std::vector<int32_t > > >::const_iterator it = result.partitions.begin();
                     it != result.partitions.end(); ++it) {
                    boost::shared_ptr<Address> addr(new Address(it->first));
                    for (std::vector<int32_t>::const_iterator partIt = it->second.begin(); partIt != it->second.end(); ++partIt) {
                        partitions.put(*partIt, addr);
                    }
                }

                partitionCount = (int)partitions.size();

                return partitionCount > 0;
            }

            bool PartitionService::getInitialPartitions() {
                bool result = false;
                std::vector<Member> memberList = clientContext.getClusterService().getMemberList();
                for (std::vector<Member>::iterator it = memberList.begin(); it < memberList.end(); ++it) {
                    const Address &target = (*it).getAddress();
                    std::auto_ptr<protocol::ClientMessage> response = getPartitionsFrom(target);
                    if (response.get() != NULL) {
                        result = processPartitionResponse(*response);

                        if (result) {
                            break;
                        }
                    }
                }

                if (!result) {
                    util::ILogger::getLogger().severe("PartitionService::getInitialPartitions Cannot get initial partitions!");
                } else {
                    util::ILogger::getLogger().finest("PartitionService::getInitialPartitions Got " +
                                                              util::IOUtil::to_string<int>(partitionCount) +
                                                              " initial partitions successfully.");
                }
                return result;
            }


            int PartitionService::getPartitionCount() {
                return partitionCount;
            }

            // TODO: Implement using executor as done in java
            void PartitionService::refreshPartitions() {
                util::Thread t(refreshTask, this);
            }

            void PartitionService::refreshTask(util::ThreadArgs &args) {
                PartitionService *partitionSrv = (PartitionService *)args.arg0;
                partitionSrv->runRefresher();
            }
        }
    }
}

