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

#include "hazelcast/client/protocol/codec/ListClearCodec.h"
#include "hazelcast/client/protocol/exception/UnexpectedMessageTypeException.h"
#include "hazelcast/util/ILogger.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const ListMessageType ListClearCodec::RequestParameters::TYPE = LIST_CLEAR;
                const bool ListClearCodec::RequestParameters::RETRYABLE = false;
                const int32_t ListClearCodec::ResponseParameters::TYPE = 100;

                std::auto_ptr<ClientMessage> ListClearCodec::RequestParameters::encode(
                        const std::string &name) {
                    int32_t requiredDataSize = calculateDataSize(name);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t)ListClearCodec::RequestParameters::TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t ListClearCodec::RequestParameters::calculateDataSize(
                        const std::string &name) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    return dataSize;
                }

                ListClearCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {
                    if (TYPE != clientMessage.getMessageType()) {
                        throw exception::UnexpectedMessageTypeException("ListClearCodec::ResponseParameters::decode", clientMessage.getMessageType(), TYPE);
                    }
                }

                ListClearCodec::ResponseParameters ListClearCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return ListClearCodec::ResponseParameters(clientMessage);
                }

                ListClearCodec::ResponseParameters::ResponseParameters(const ListClearCodec::ResponseParameters &rhs) {
                }
                //************************ EVENTS END **************************************************************************//

            }
        }
    }
}

