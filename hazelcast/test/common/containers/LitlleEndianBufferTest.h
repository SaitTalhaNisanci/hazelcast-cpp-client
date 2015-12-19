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
// Created by İhsan Demir on 17/05/15.
//

#ifndef HAZELCAST_CLIENT_COMMON_CONTAINERS_LITLLEENDIANBUFFERTEST_H
#define HAZELCAST_CLIENT_COMMON_CONTAINERS_LITLLEENDIANBUFFERTEST_H

#include "HazelcastServerFactory.h"
#include <iTest/iTestFixture.h>
#include "hazelcast/util/LittleEndianBufferWrapper.h"

namespace hazelcast {
    namespace client {
        namespace test {
            class HazelcastServerFactory;

            namespace common {
                namespace containers {
                    class LittleEndianBufferTest : public iTest::iTestFixture<LittleEndianBufferTest>,
                                                   public util::LittleEndianBufferWrapper /* Need this in order to test*/ {
                    public:
                        LittleEndianBufferTest();

                        ~LittleEndianBufferTest();

                        void addTests();

                        void beforeClass();

                        void afterClass();

                        void beforeTest();

                        void afterTest();

/*
                        void testBinaryFormat();
*/

                    };
                }
            }
        }
    }
}


#endif //HAZELCAST_CLIENT_COMMON_CONTAINERS_LITLLEENDIANBUFFERTEST_H
