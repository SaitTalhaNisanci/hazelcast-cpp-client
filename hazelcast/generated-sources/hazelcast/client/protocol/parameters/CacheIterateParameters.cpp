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

#include "hazelcast/client/protocol/parameters/CacheIterateParameters.h"
#include "hazelcast/client/protocol/ClientMessageType.h"
#include "hazelcast/client/protocol/ClientMessage.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace parameters {
                    std::auto_ptr<ClientMessage> CacheIterateParameters::encode(
                            const std::string &name, 
                            const int32_t &partitionId, 
                            const int32_t &tableIndex, 
                            const int32_t &batch) {
                        int32_t requiredDataSize = calculateDataSize(name, partitionId, tableIndex, batch);
                        std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                        clientMessage->setMessageType((uint16_t)CacheIterateParameters::TYPE);
                        clientMessage->set(name).set(partitionId).set(tableIndex).set(batch);
                        clientMessage->updateFrameLength();
                        return clientMessage;
                    }

                    int32_t CacheIterateParameters::calculateDataSize(
                            const std::string &name, 
                            const int32_t &partitionId, 
                            const int32_t &tableIndex, 
                            const int32_t &batch) {
                        return ClientMessage::HEADER_SIZE
                             + ClientMessage::calculateDataSize(name)
                             + ClientMessage::calculateDataSize(partitionId)
                             + ClientMessage::calculateDataSize(tableIndex)
                             + ClientMessage::calculateDataSize(batch);
                    }
            }
        }
    }
}

