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
#ifndef HAZELCAST_CLIENT_PROTOCOL_CODEC_CLIENTREMOVEPARTITIONLOSTLISTENERCODEC_H_
#define HAZELCAST_CLIENT_PROTOCOL_CODEC_CLIENTREMOVEPARTITIONLOSTLISTENERCODEC_H_

#include <memory>


#include "hazelcast/client/protocol/codec/ClientMessageType.h"
#include "hazelcast/util/HazelcastDll.h"
#include "hazelcast/client/impl/BaseEventHandler.h"
#include "hazelcast/client/protocol/ClientMessage.h"
#include "hazelcast/client/protocol/codec/IRemoveListenerCodec.h"


namespace hazelcast {
    namespace client {

        namespace protocol {
            namespace codec {
                class HAZELCAST_API ClientRemovePartitionLostListenerCodec : public IRemoveListenerCodec{
                public:
                    //************************ REQUEST STARTS ******************************************************************//
                    class RequestParameters {
                        public:
                            static const enum ClientMessageType TYPE;
                            static const bool RETRYABLE;

                        static std::auto_ptr<ClientMessage> encode(
                                const std::string &registrationId);

                        static int32_t calculateDataSize(
                                const std::string &registrationId);

                        private:
                            // Preventing public access to constructors
                            RequestParameters();
                    };
                    //************************ REQUEST ENDS ********************************************************************//

                    //************************ RESPONSE STARTS *****************************************************************//
                    class ResponseParameters {
                        public:
                            static const int TYPE;

                            bool response;
                            
                            static ResponseParameters decode(ClientMessage &clientMessage);

                            // define copy constructor (needed for auto_ptr variables)
                            ResponseParameters(const ResponseParameters &rhs);
                        private:
                            ResponseParameters(ClientMessage &clientMessage);
                    };
                    //************************ RESPONSE ENDS *******************************************************************//

                    ClientRemovePartitionLostListenerCodec (const std::string &registrationId);

                    //************************ IRemoveListenerCodec interface starts *******************************************//
                    std::auto_ptr<ClientMessage> encodeRequest() const;

                    bool decodeResponse(ClientMessage &clientMessage) const;

                    const std::string &getRegistrationId() const;

                    void setRegistrationId(const std::string &id);

                    //************************ IRemoveListenerCodec interface ends *********************************************//
                    private:
                        // Preventing public access to constructors
                        ClientRemovePartitionLostListenerCodec ();

                        std::string registrationId_;
                };
            }
        }
    }
}

#endif /* HAZELCAST_CLIENT_PROTOCOL_CODEC_CLIENTREMOVEPARTITIONLOSTLISTENERCODEC_H_ */
