/*
 * Copyright (c) 2008-2018, Hazelcast, Inc. All Rights Reserved.
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
// Created by sancar koyunlu on 04/09/14.
//

#include "hazelcast/client/MapEvent.h"

namespace hazelcast {
    namespace client {

        MapEvent::MapEvent(const Member& member, EntryEventType eventType, const std::string& name, int numberOfEntriesAffected)
        : member(member)
        , eventType(eventType)
        , name(name)
        , numberOfEntriesAffected(numberOfEntriesAffected) {

        }

        Member MapEvent::getMember() const {
            return member;
        }

        EntryEventType MapEvent::getEventType() const {
            return eventType;
        }

        const std::string& MapEvent::getName() const {
            return name;
        }

        int MapEvent::getNumberOfEntriesAffected() const {
            return numberOfEntriesAffected;
        }

        std::ostream &operator<<(std::ostream &os, const MapEvent &event) {
            os << "MapEvent{member: " << event.member << " eventType: " << event.eventType << " name: " << event.name
               << " numberOfEntriesAffected: " << event.numberOfEntriesAffected;
            return os;
        }
    }
}



