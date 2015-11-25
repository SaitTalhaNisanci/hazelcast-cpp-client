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

#include "hazelcast/client/protocol/codec/ConditionBeforeAwaitCodec.h"
#include "hazelcast/client/protocol/exception/UnexpectedMessageTypeException.h"
#include "hazelcast/util/ILogger.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const ConditionMessageType ConditionBeforeAwaitCodec::RequestParameters::TYPE = CONDITION_BEFOREAWAIT;
                const bool ConditionBeforeAwaitCodec::RequestParameters::RETRYABLE = false;
                const int32_t ConditionBeforeAwaitCodec::ResponseParameters::TYPE = 100;

                std::auto_ptr<ClientMessage> ConditionBeforeAwaitCodec::RequestParameters::encode(
                        const std::string &name, 
                        int64_t threadId, 
                        const std::string &lockName) {
                    int32_t requiredDataSize = calculateDataSize(name, threadId, lockName);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t)ConditionBeforeAwaitCodec::RequestParameters::TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->set(threadId);
                    clientMessage->set(lockName);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t ConditionBeforeAwaitCodec::RequestParameters::calculateDataSize(
                        const std::string &name, 
                        int64_t threadId, 
                        const std::string &lockName) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize(threadId);
                    dataSize += ClientMessage::calculateDataSize(lockName);
                    return dataSize;
                }

                ConditionBeforeAwaitCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {
                    if (TYPE != clientMessage.getMessageType()) {
                        throw exception::UnexpectedMessageTypeException("ConditionBeforeAwaitCodec::ResponseParameters::decode", clientMessage.getMessageType(), TYPE);
                    }
                }

                ConditionBeforeAwaitCodec::ResponseParameters ConditionBeforeAwaitCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return ConditionBeforeAwaitCodec::ResponseParameters(clientMessage);
                }

                ConditionBeforeAwaitCodec::ResponseParameters::ResponseParameters(const ConditionBeforeAwaitCodec::ResponseParameters &rhs) {
                }
                //************************ EVENTS END **************************************************************************//

            }
        }
    }
}

