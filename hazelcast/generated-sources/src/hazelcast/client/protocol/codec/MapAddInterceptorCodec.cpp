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

#include "hazelcast/client/protocol/codec/MapAddInterceptorCodec.h"
#include "hazelcast/client/protocol/exception/UnexpectedMessageTypeException.h"
#include "hazelcast/util/ILogger.h"
#include "hazelcast/client/serialization/pimpl/Data.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                const MapMessageType MapAddInterceptorCodec::RequestParameters::TYPE = MAP_ADDINTERCEPTOR;
                const bool MapAddInterceptorCodec::RequestParameters::RETRYABLE = false;
                const int32_t MapAddInterceptorCodec::ResponseParameters::TYPE = 104;

                std::auto_ptr<ClientMessage> MapAddInterceptorCodec::RequestParameters::encode(
                        const std::string &name, 
                        const serialization::pimpl::Data &interceptor) {
                    int32_t requiredDataSize = calculateDataSize(name, interceptor);
                    std::auto_ptr<ClientMessage> clientMessage = ClientMessage::createForEncode(requiredDataSize);
                    clientMessage->setMessageType((uint16_t)MapAddInterceptorCodec::RequestParameters::TYPE);
                    clientMessage->setRetryable(RETRYABLE);
                    clientMessage->set(name);
                    clientMessage->set(interceptor);
                    clientMessage->updateFrameLength();
                    return clientMessage;
                }

                int32_t MapAddInterceptorCodec::RequestParameters::calculateDataSize(
                        const std::string &name, 
                        const serialization::pimpl::Data &interceptor) {
                    int32_t dataSize = ClientMessage::HEADER_SIZE;
                    dataSize += ClientMessage::calculateDataSize(name);
                    dataSize += ClientMessage::calculateDataSize(interceptor);
                    return dataSize;
                }

                MapAddInterceptorCodec::ResponseParameters::ResponseParameters(ClientMessage &clientMessage) {
                    if (TYPE != clientMessage.getMessageType()) {
                        throw exception::UnexpectedMessageTypeException("MapAddInterceptorCodec::ResponseParameters::decode", clientMessage.getMessageType(), TYPE);
                    }

                    response = clientMessage.get<std::string >();
                }

                MapAddInterceptorCodec::ResponseParameters MapAddInterceptorCodec::ResponseParameters::decode(ClientMessage &clientMessage) {
                    return MapAddInterceptorCodec::ResponseParameters(clientMessage);
                }

                MapAddInterceptorCodec::ResponseParameters::ResponseParameters(const MapAddInterceptorCodec::ResponseParameters &rhs) {
                        response = rhs.response;
                }
                //************************ EVENTS END **************************************************************************//

            }
        }
    }
}

