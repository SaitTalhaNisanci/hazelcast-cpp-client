//
// Created by sancar koyunlu on 5/23/13.
// Copyright (c) 2013 hazelcast. All rights reserved.

#include "MembershipEvent.h"
#include "../protocol/AddMembershipListenerRequest.h"
#include "../../util/SerializableCollection.h"
#include "../ClientConfig.h"
#include "../spi/ClusterService.h"
#include "../MembershipListener.h"

namespace hazelcast {
    namespace client {
        namespace connection {
            ClusterListenerThread::ClusterListenerThread(ConnectionManager& connectionMgr, ClientConfig& clientConfig, spi::ClusterService& clusterService)
            : Thread::Thread(connection::ClusterListenerThread::run, this)
            , connectionManager(connectionMgr)
            , clientConfig(clientConfig)
            , clusterService(clusterService)
            , conn(NULL) {
                ;
            };

            void ClusterListenerThread::setInitialConnection(connection::Connection *connection) {
                this->conn = connection;
            };

            void *ClusterListenerThread::run(void *input) {
                static_cast<ClusterListenerThread *>(input)->runImpl();
                return NULL;

            };

            void ClusterListenerThread::runImpl() {
                while (true) {
                    try {
                        if (conn == NULL) {
                            conn = pickConnection();
                            std::cout << "Connected: " << (*conn) << std::endl;
                        }
                        loadInitialMemberList();
                        listenMembershipEvents();
                    } catch (HazelcastException& e) {
//                        if (client.getLifecycleService().isRunning()) {
//                            e.printStackTrace();
//                        }
                        std::cout << *conn << " FAILED..." << std::endl;
                        conn->close();
                    }
                    try {
                        sleep(1);
                    } catch (void *x) {
                        break;
                    }
                }
            };

            Connection *ClusterListenerThread::pickConnection() {
                std::vector<Address> addresses;
                if (!members.empty()) {
                    vector<Address> clusterAddresses = getClusterAddresses();
                    addresses.insert(addresses.begin(), clusterAddresses.begin(), clusterAddresses.end());
                }
                vector<Address> configAddresses = getConfigAddresses();
                addresses.insert(addresses.end(), configAddresses.begin(), configAddresses.end());
//                std::cout << "Possible addresses: " << addresses << std::endl;
                return clusterService.connectToOne(addresses);
            };

            void ClusterListenerThread::loadInitialMemberList() {
                protocol::AddMembershipListenerRequest request;
                util::SerializableCollection coll = clusterService.sendAndReceive<util::SerializableCollection>(*conn, request);

                serialization::SerializationService & serializationService = clusterService.getSerializationService();
                std::map<std::string, Member> prevMembers;
                if (!members.empty()) {
                    for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                        prevMembers[(*it).getUuid()] = *it;
                    }
                    members.clear();
                }

                vector<serialization::Data *> collection = coll.getCollection();
                for (vector<serialization::Data *>::iterator it = collection.begin(); it != collection.end(); ++it) {
                    Member member = serializationService.toObject<Member>(*(*it));

                    members.push_back(member);
                }
                updateMembersRef();
                std::vector<MembershipEvent> events;

                for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                    if (prevMembers.count((*it).getUuid()) > 0) {
                        prevMembers.erase((*it).getUuid());
                    } else {
                        events.push_back(MembershipEvent((*it), MembershipEvent::MEMBER_ADDED));
                    }
                }

                for (std::map< std::string, Member >::iterator it = prevMembers.begin(); it != prevMembers.end(); ++it) {
                    events.push_back(MembershipEvent(it->second, MembershipEvent::MEMBER_REMOVED));
                }


                for (std::vector<MembershipEvent>::iterator it = events.begin(); it != events.end(); ++it) {
                    fireMembershipEvent((*it));
                }
            };

            void ClusterListenerThread::listenMembershipEvents() {
                serialization::SerializationService & serializationService = clusterService.getSerializationService();
                while (true) {
                    serialization::Data data = conn->read(serializationService.getSerializationContext());
                    MembershipEvent event = serializationService.toObject<MembershipEvent>(data);
                    Member member = event.getMember();
                    if (event.getEventType() == MembershipEvent::MEMBER_ADDED) {
                        members.push_back(member);
                    } else {
                        //TODO O(n) time complexity ???
                        members.erase(std::find(members.begin(), members.end(), member));
                    }
                    updateMembersRef();
//                    connectionManager.removeConnectionPool(member.getAddress());//TODO why remove
                    fireMembershipEvent(event);
                }
            };


            void ClusterListenerThread::fireMembershipEvent(MembershipEvent & event) {
                MembershipListener *listener = dynamic_cast<MembershipListener *>(clusterService.getClientConfig().getLoadBalancer());
                if (event.getEventType() == MembershipEvent::MEMBER_ADDED) {
                    listener->memberAdded(event);
                } else {
                    listener->memberRemoved(event);
                }
                //TODO give this job to another thread
//                vector<MembershipListener *> listeners = clusterService.listeners.keys();
//
//                for (vector<MembershipListener *>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
//                    if (event.getEventType() == MembershipEvent::MEMBER_ADDED) {
//                        (*it)->memberAdded(event);
//                    } else {
//                        (*it)->memberRemoved(event);
//                    }
//                }
            };

            void ClusterListenerThread::updateMembersRef() {
                std::map<Address, Member> *map = new std::map<Address, Member>;
                std::cerr << "Members [" << members.size() << "]  {" << std::endl;
                for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                    std::cerr << "\t" << (*it) << std::endl;
                    (*map)[(*it).getAddress()] = (*it);
                }
                std::cerr << "}" << std::endl;
                delete clusterService.membersRef.set(map);

            };

            std::vector<Address> ClusterListenerThread::getClusterAddresses() const {
                std::vector<Address> socketAddresses;
                for (std::vector<Member>::const_iterator it = members.begin(); it != members.end(); ++it) {
                    socketAddresses.push_back((*it).getAddress());
                }
                //                Collections.shuffle(socketAddresses);
                return socketAddresses;
            };

            vector<Address>  ClusterListenerThread::getConfigAddresses() const {
                std::vector<Address> socketAddresses;
                std::vector<Address>  & configAddresses = clientConfig.getAddresses();

                for (std::vector<Address>::iterator it = configAddresses.begin(); it != configAddresses.end(); ++it) {
                    socketAddresses.push_back((*it));
                }
                //                Collections.shuffle(socketAddresses);
                return socketAddresses;
            };


        }
    }
}