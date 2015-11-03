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
#ifndef HAZELCAST_CLIENT_LISTREMOVELISTENERPARAMETERS_H_
#define HAZELCAST_CLIENT_LISTREMOVELISTENERPARAMETERS_H_

#include <memory>
#include <stdint.h>
#include <string>

#include "hazelcast/util/HazelcastDll.h"
#include "hazelcast/client/protocol/GeneratedMessageType.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;

            namespace parameters {
                class HAZELCAST_API ListRemoveListenerParameters {
                public:
                    static const protocol::GeneratedMessageType TYPE = protocol::LIST_REMOVELISTENER;

                    static std::auto_ptr<ClientMessage> encode(
                            const std::string &name, 
                            const std::string &registrationId);

                    static int32_t calculateDataSize(
                            const std::string &name, 
                            const std::string &registrationId);

                private:
                    // Preventing public access to constructors
                    ListRemoveListenerParameters ();
                };
            }
        }
    }
}

#endif /* HAZELCAST_CLIENT_LISTREMOVELISTENERPARAMETERS_H_ */
