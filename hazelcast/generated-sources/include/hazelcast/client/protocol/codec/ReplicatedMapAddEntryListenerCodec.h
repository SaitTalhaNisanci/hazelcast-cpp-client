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
#ifndef HAZELCAST_CLIENT_PROTOCOL_CODEC_REPLICATEDMAPADDENTRYLISTENERCODEC_H_
#define HAZELCAST_CLIENT_PROTOCOL_CODEC_REPLICATEDMAPADDENTRYLISTENERCODEC_H_

#include <memory>


#include "hazelcast/client/protocol/codec/ReplicatedMapMessageType.h"
#include "hazelcast/util/HazelcastDll.h"
#include "hazelcast/client/impl/BaseEventHandler.h"
#include "hazelcast/client/protocol/ClientMessage.h"
#include "hazelcast/client/protocol/codec/IAddListenerCodec.h"


namespace hazelcast {
    namespace client {
        namespace serialization {
            namespace pimpl {
                class Data;
            }
        }

        namespace protocol {
            namespace codec {
                class HAZELCAST_API ReplicatedMapAddEntryListenerCodec : public IAddListenerCodec{
                public:
                    //************************ REQUEST STARTS ******************************************************************//
                    class RequestParameters {
                        public:
                            static const enum ReplicatedMapMessageType TYPE;
                            static const bool RETRYABLE;

                        static std::auto_ptr<ClientMessage> encode(
                                const std::string &name, 
                                bool localOnly);

                        static int32_t calculateDataSize(
                                const std::string &name, 
                                bool localOnly);

                        private:
                            // Preventing public access to constructors
                            RequestParameters();
                    };
                    //************************ REQUEST ENDS ********************************************************************//

                    //************************ RESPONSE STARTS *****************************************************************//
                    class ResponseParameters {
                        public:
                            static const int TYPE;

                            std::string response;
                            
                            static ResponseParameters decode(ClientMessage &clientMessage);

                            // define copy constructor (needed for auto_ptr variables)
                            ResponseParameters(const ResponseParameters &rhs);
                        private:
                            ResponseParameters(ClientMessage &clientMessage);
                    };
                    //************************ RESPONSE ENDS *******************************************************************//

                    //************************ EVENTS START*********************************************************************//
                    class AbstractEventHandler : public impl::BaseEventHandler {
                        public:
                        void handle(std::auto_ptr<protocol::ClientMessage> message);

                        virtual void handleEntry(std::auto_ptr<serialization::pimpl::Data > key, std::auto_ptr<serialization::pimpl::Data > value, std::auto_ptr<serialization::pimpl::Data > oldValue, std::auto_ptr<serialization::pimpl::Data > mergingValue, const int32_t &eventType, const std::string &uuid, const int32_t &numberOfAffectedEntries) = 0;

                    };

                    //************************ EVENTS END **********************************************************************//

                    ReplicatedMapAddEntryListenerCodec (const std::string &name, const bool &localOnly);

                    //************************ IAddListenerCodec interface starts *******************************************//
                    std::auto_ptr<ClientMessage> encodeRequest() const;

                    std::string decodeResponse(ClientMessage &responseMessage) const;

                    //************************ IAddListenerCodec interface ends *********************************************//
                    private:
                        // Preventing public access to constructors
                        ReplicatedMapAddEntryListenerCodec ();

                        std::string name_;
                        bool localOnly_;
                };
            }
        }
    }
}

#endif /* HAZELCAST_CLIENT_PROTOCOL_CODEC_REPLICATEDMAPADDENTRYLISTENERCODEC_H_ */
