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

#include "hazelcast/client/protocol/parameters/MultiMapGetParameters.h"
#include "hazelcast/client/protocol/ClientMessageType.h"
#include "hazelcast/client/protocol/ClientMessage.h"
#include "hazelcast/client/serialization/pimpl/Data.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace parameters {
                    std::auto_ptr<ClientMessage> MultiMapGetParameters::encode(
                            const std::string &name, 
                            const serialization::pimpl::Data &key, 
                            const int64_t &threadId) {
                        int32_t requiredDataSize = calculateDataSize(name, key, threadId);
                        std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                        clientMessage->setMessageType((uint16_t)MultiMapGetParameters::TYPE);
                        clientMessage->set(name).set(key).set(threadId);
                        clientMessage->updateFrameLength();
                        return clientMessage;
                    }

                    int32_t MultiMapGetParameters::calculateDataSize(
                            const std::string &name, 
                            const serialization::pimpl::Data &key, 
                            const int64_t &threadId) {
                        return ClientMessage::HEADER_SIZE
                             + ClientMessage::calculateDataSize(name)
                             + ClientMessage::calculateDataSize(key)
                             + ClientMessage::calculateDataSize(threadId);
                    }
            }
        }
    }
}

