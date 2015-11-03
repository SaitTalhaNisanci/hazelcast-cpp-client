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
 * DataEntryListResultParameters.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: ihsan
 */

#include "hazelcast/client/protocol/parameters/DataEntryListResultParameters.h"

#include "hazelcast/client/protocol/ClientMessage.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace parameters {
                std::auto_ptr<DataEntryListResultParameters> DataEntryListResultParameters::decode(ClientMessage &message) {
                    return std::auto_ptr<DataEntryListResultParameters>(new DataEntryListResultParameters(message));
                }

                DataEntryListResultParameters::DataEntryListResultParameters(ClientMessage &message) :
                        keys(message.getDataList()), values(message.getDataList()) {

                    assert(TYPE == message.getMessageType());
                }
            }
        }
    }
}
