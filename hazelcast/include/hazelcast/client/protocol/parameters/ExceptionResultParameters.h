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
 * ExceptionResultParameters.h
 *
 *  Created on: Apr 13, 2015
 *      Author: ihsan
 */

#ifndef HAZELCAST_CLIENT_EXCEPTIONRESULTPARAMETERS_H_
#define HAZELCAST_CLIENT_EXCEPTIONRESULTPARAMETERS_H_

#include <string>
#include <memory>

#include "hazelcast/client/protocol/ClientMessageType.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class ClientMessage;

            namespace parameters {
                class ExceptionResultParameters {
                public:
                    std::auto_ptr<std::string> className;
                    std::auto_ptr<std::string> message;
                    std::auto_ptr<std::string> stacktrace;

                    static const protocol::ClientMessageType TYPE = protocol::EXCEPTION;

                    /**
                    * Decode input byte array data into parameters
                    *
                    * @param message
                    * @return ExceptionResultParameters
                    */
                    static std::auto_ptr<ExceptionResultParameters> decode(ClientMessage &message);

                private:
                    ExceptionResultParameters(ClientMessage &message);
                };
            }
        }
    }
}

#endif /* HAZELCAST_CLIENT_EXCEPTIONRESULTPARAMETERS_H_ */
