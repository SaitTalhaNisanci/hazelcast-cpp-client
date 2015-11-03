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
//
// Created by sancar koyunlu on 28/01/14.
//

#include "hazelcast/client/MemberAttributeEvent.h"
#include "hazelcast/client/Cluster.h"

namespace hazelcast {
    namespace client {

        MemberAttributeEvent::MemberAttributeEvent(Cluster &cluster, Member &member, MemberAttributeOperationType operationType, const std::string &key, const std::string &value)
        :MembershipEvent(cluster, MembershipEvent::MEMBER_ADDED, member)
        , operationType(operationType)
        , key(key)
        , value(value) {

        }

        MemberAttributeEvent::MemberAttributeOperationType MemberAttributeEvent::getOperationType() const {
            return operationType;
        }

        const std::string &MemberAttributeEvent::getKey() const {
            return key;
        }
        const std::string &MemberAttributeEvent::getValue() const {
            return value;
        }
    }
}
