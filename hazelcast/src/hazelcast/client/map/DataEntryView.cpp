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
// Created by sancar koyunlu on 20/02/14.
//

#include "hazelcast/client/map/DataEntryView.h"
#include "hazelcast/client/map/DataSerializableHook.h"
#include "hazelcast/client/serialization/ObjectDataInput.h"

namespace hazelcast {
    namespace client {
        namespace map{
                int DataEntryView::getFactoryId() const {
                    return map::DataSerializableHook::F_ID;
                }
                int DataEntryView::getClassId() const {
                    return map::DataSerializableHook::ENTRY_VIEW;
                }
                void DataEntryView::readData(serialization::ObjectDataInput &in) {
                    if(in.readBoolean()){
                        key = in.readData();
                    }
                    if(in.readBoolean()){
                        value = in.readData();
                    }
                    cost = in.readLong();
                    creationTime = in.readLong();
                    expirationTime = in.readLong();
                    hits = in.readLong();
                    lastAccessTime = in.readLong();
                    lastStoredTime = in.readLong();
                    lastUpdateTime = in.readLong();
                    version = in.readLong();
                }
        }
    }
}

