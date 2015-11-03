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
// Created by İhsan Demir on 18/05/15.
//


#ifndef HAZELCAST_CLIENT_TEST_UTIL_BITS_H
#define HAZELCAST_CLIENT_TEST_UTIL_BITS_H


#include "iTest/iTest.h"

namespace hazelcast {
    namespace client {
        namespace test {
            namespace util {
                class BitsTest : public iTest::iTestFixture<BitsTest> {
                public:

                    BitsTest();

                    void addTests();

                    void beforeClass();

                    void afterClass();

                    void beforeTest();

                    void afterTest();

                    void testLittleEndian();

                };
            }
        }
    }
}


#endif //HAZELCAST_CLIENT_TEST_UTIL_BITS_H

