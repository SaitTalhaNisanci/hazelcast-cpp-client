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

/*
 * AuthenticationCustomCredentialsParameters.h
 *
 *  Created on: Apr 10, 2015
 *      Author: ihsan
 */

#ifndef HAZELCAST_CLIENT_AUTHENTICATIONCUSTOMCREDENTIALSPARAMETERS_H_
#define HAZELCAST_CLIENT_AUTHENTICATIONCUSTOMCREDENTIALSPARAMETERS_H_

#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

#include "hazelcast/util/HazelcastDll.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;

            namespace parameters {
                class AuthenticationCustomCredentialsParameters {
                public:

                    virtual ~AuthenticationCustomCredentialsParameters();

                    /**
                    * Encode parameters into byte array, i.e. ClientMessage
                    *
                    * @param credentials
                    * @param ownerUuid
                    * @param isOwnerConnection
                    * @return encoded ClientMessage
                    */
                    static std::auto_ptr<ClientMessage> encode(std::vector<byte> &credentials,
                            const std::string & uuid,
                            const std::string & ownerUuid,
                            uint8_t isOwnerConnection);

                    /**
                    * sample data size estimation
                    *
                    * @return size
                    */
                    static int32_t calculateDataSize(std::vector<byte> &credentials,
                            const std::string & uuid,
                            const std::string & ownerUuid,
                            uint8_t isOwnerConnection);

                private:
                    // Hide constructor
                    AuthenticationCustomCredentialsParameters();
                };
            }
        }
    }
}

#endif /* HAZELCAST_CLIENT_AUTHENTICATIONCUSTOMCREDENTIALSPARAMETERS_H_ */
