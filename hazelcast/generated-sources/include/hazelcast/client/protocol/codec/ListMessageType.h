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
#ifndef HAZELCAST_CLIENT_PROTOCOL_CODEC_LISTMESSAGETYPE
#define HAZELCAST_CLIENT_PROTOCOL_CODEC_LISTMESSAGETYPE

namespace hazelcast {
    namespace client {
        namespace protocol {
            namespace codec {
                enum ListMessageType {

                    LIST_SIZE=0x0501,
                    LIST_CONTAINS=0x0502,
                    LIST_CONTAINSALL=0x0503,
                    LIST_ADD=0x0504,
                    LIST_REMOVE=0x0505,
                    LIST_ADDALL=0x0506,
                    LIST_COMPAREANDREMOVEALL=0x0507,
                    LIST_COMPAREANDRETAINALL=0x0508,
                    LIST_CLEAR=0x0509,
                    LIST_GETALL=0x050a,
                    LIST_ADDLISTENER=0x050b,
                    LIST_REMOVELISTENER=0x050c,
                    LIST_ISEMPTY=0x050d,
                    LIST_ADDALLWITHINDEX=0x050e,
                    LIST_GET=0x050f,
                    LIST_SET=0x0510,
                    LIST_ADDWITHINDEX=0x0511,
                    LIST_REMOVEWITHINDEX=0x0512,
                    LIST_LASTINDEXOF=0x0513,
                    LIST_INDEXOF=0x0514,
                    LIST_SUB=0x0515,
                    LIST_ITERATOR=0x0516,
                    LIST_LISTITERATOR=0x0517
                };
            }
        }
    }
}

#endif // HAZELCAST_CLIENT_PROTOCOL_CODEC_LISTMESSAGETYPE




