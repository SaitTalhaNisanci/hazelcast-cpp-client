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
// Created by sancar koyunlu on 26/02/14.
//


#include "cluster/ClusterTest.h"
#include "hazelcast/client/HazelcastClient.h"
#include "hazelcast/client/InitialMembershipEvent.h"
#include "hazelcast/client/InitialMembershipListener.h"
#include "hazelcast/client/MemberAttributeEvent.h"
#include "hazelcast/client/EntryAdapter.h"
#include "HazelcastServerFactory.h"
#include "HazelcastServer.h"
#include "hazelcast/client/LifecycleListener.h"

namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {
            using namespace iTest;

            ClusterTest::ClusterTest(HazelcastServerFactory& hazelcastInstanceFactory)
            : ClientTestSupport<ClusterTest>("ClusterTest" , &hazelcastInstanceFactory), hazelcastInstanceFactory(hazelcastInstanceFactory) {
            }


            ClusterTest::~ClusterTest() {
            }

            void ClusterTest::addTests() {
                addTest(&ClusterTest::testClusterListeners, "testClusterListeners");
                addTest(&ClusterTest::testClusterListenersFromConfig, "testClusterListenersFromConfig");
//                addTest(&ClusterTest::testListenersWhenClusterDown, "testListenersWhenClusterDown");
                addTest(&ClusterTest::testBehaviourWhenClusterNotFound, "testBehaviourWhenClusterNotFound");
            }

            void ClusterTest::beforeClass() {

            }

            void ClusterTest::afterClass() {

            }

            void ClusterTest::beforeTest() {

            }

            void ClusterTest::afterTest() {

            }

            class SampleInitialListener : public InitialMembershipListener {
            public:
                SampleInitialListener(util::CountDownLatch& _memberAdded, util::CountDownLatch& _attributeLatch, util::CountDownLatch& _memberRemoved)
                : _memberAdded(_memberAdded), _attributeLatch(_attributeLatch), _memberRemoved(_memberRemoved) {

                }

                void init(const InitialMembershipEvent& event) {
                    std::vector<Member> const& members = event.getMembers();
                    if (members.size() == 1) {
                        _memberAdded.countDown();
                    }
                }

                void memberAdded(const MembershipEvent& event) {
                    _memberAdded.countDown();
                }

                void memberRemoved(const MembershipEvent& event) {
                    _memberRemoved.countDown();
                }


                void memberAttributeChanged(const MemberAttributeEvent& memberAttributeEvent) {
                    _attributeLatch.countDown();
                }

            private:
                util::CountDownLatch& _memberAdded;
                util::CountDownLatch& _attributeLatch;
                util::CountDownLatch& _memberRemoved;
            };


            class SampleListenerInClusterTest : public MembershipListener {
            public:
                SampleListenerInClusterTest(util::CountDownLatch& _memberAdded, util::CountDownLatch& _attributeLatch, util::CountDownLatch& _memberRemoved)
                : _memberAdded(_memberAdded), _attributeLatch(_attributeLatch), _memberRemoved(_memberRemoved) {

                }

                void memberAdded(const MembershipEvent& event) {
                    _memberAdded.countDown();
                }

                void memberRemoved(const MembershipEvent& event) {
                    _memberRemoved.countDown();
                }

                void memberAttributeChanged(const MemberAttributeEvent& memberAttributeEvent) {
                    memberAttributeEvent.getKey();
                    _attributeLatch.countDown();
                }

            private:
                util::CountDownLatch& _memberAdded;
                util::CountDownLatch& _attributeLatch;
                util::CountDownLatch& _memberRemoved;
            };

            void ClusterTest::testClusterListeners() {
                HazelcastServer instance(hazelcastInstanceFactory);
                std::auto_ptr<HazelcastClient> hazelcastClient(getNewClient());
                Cluster cluster = hazelcastClient->getCluster();
                util::CountDownLatch memberAdded(1);
                util::CountDownLatch memberAddedInit(2);
                util::CountDownLatch memberRemoved(1);
                util::CountDownLatch memberRemovedInit(1);
                util::CountDownLatch attributeLatch(7);
                util::CountDownLatch attributeLatchInit(7);

                SampleInitialListener sampleInitialListener(memberAddedInit, attributeLatchInit, memberRemovedInit);
                SampleListenerInClusterTest sampleListener(memberAdded, attributeLatch, memberRemoved);

                cluster.addMembershipListener(&sampleInitialListener);
                cluster.addMembershipListener(&sampleListener);

                HazelcastServer instance2(hazelcastInstanceFactory);

                assertTrue(attributeLatchInit.await(30), "attributeLatchInit");
                assertTrue(attributeLatch.await(30), "attributeLatch");
                assertTrue(memberAdded.await(30), "memberAdded");
                assertTrue(memberAddedInit.await(30), "memberAddedInit");

                instance2.shutdown();

                assertTrue(memberRemoved.await(30), "memberRemoved");
                assertTrue(memberRemovedInit.await(30), "memberRemovedInit");

                instance.shutdown();

                cluster.removeMembershipListener(&sampleInitialListener);
                cluster.removeMembershipListener(&sampleListener);
            }

            void ClusterTest::testClusterListenersFromConfig() {
                util::CountDownLatch memberAdded(2);
                util::CountDownLatch memberAddedInit(3);
                util::CountDownLatch memberRemoved(1);
                util::CountDownLatch memberRemovedInit(1);
                util::CountDownLatch attributeLatch(7);
                util::CountDownLatch attributeLatchInit(7);
                SampleInitialListener sampleInitialListener(memberAddedInit, attributeLatchInit, memberRemovedInit);
                SampleListenerInClusterTest sampleListener(memberAdded, attributeLatch, memberRemoved);

                std::auto_ptr<ClientConfig> clientConfig(getConfig());
                clientConfig->addListener(&sampleListener);
                clientConfig->addListener(&sampleInitialListener);

                HazelcastServer instance(hazelcastInstanceFactory);
                HazelcastClient hazelcastClient(*clientConfig);

                HazelcastServer instance2(hazelcastInstanceFactory);

                assertTrue(attributeLatchInit.await(30), "attributeLatchInit");
                assertTrue(attributeLatch.await(30), "attributeLatch");
                assertTrue(memberAdded.await(30), "memberAdded");
                assertTrue(memberAddedInit.await(30), "memberAddedInit");

                instance2.shutdown();

                assertTrue(memberRemoved.await(30), "memberRemoved");
                assertTrue(memberRemovedInit.await(30), "memberRemovedInit");

                instance.shutdown();
            }

            class DummyListenerClusterTest : public EntryAdapter<std::string, std::string> {
            public:
                DummyListenerClusterTest(util::CountDownLatch& addLatch)
                : addLatch(addLatch) {
                }

                void entryAdded(const EntryEvent<std::string, std::string>& event) {
                    addLatch.countDown();
                }

            private:
                util::CountDownLatch& addLatch;
            };

            class LclForClusterTest : public LifecycleListener {
            public:
                LclForClusterTest(util::CountDownLatch& latch)
                : latch(latch) {

                }

                void stateChanged(const LifecycleEvent& event) {
                    if (event.getState() == LifecycleEvent::CLIENT_CONNECTED) {
                        latch.countDown();
                    }
                }

            private:
                util::CountDownLatch& latch;
            };

            void ClusterTest::testListenersWhenClusterDown() {
                HazelcastServer instance(hazelcastInstanceFactory);

                std::auto_ptr<ClientConfig> clientConfig(getConfig());
                clientConfig->setAttemptPeriod(1000 * 10).setConnectionAttemptLimit(100).setLogLevel(FINEST);
                HazelcastClient hazelcastClient(*clientConfig);

                util::CountDownLatch countDownLatch(1);
                DummyListenerClusterTest listener(countDownLatch);
                IMap<std::string, std::string> m = hazelcastClient.getMap<std::string, std::string>("testListenersWhenClusterDown");
                m.addEntryListener(listener, true);
                instance.shutdown();

                util::CountDownLatch lifecycleLatch(1);
                LclForClusterTest lifecycleListener(lifecycleLatch);
                hazelcastClient.addLifecycleListener(&lifecycleListener);

                HazelcastServer instance2(hazelcastInstanceFactory);
                assertTrue(lifecycleLatch.await(120), "Lifecycle latch await timed out!");
                m.put("sample", "entry");
                assertTrue(countDownLatch.await(60), "Await timed out !");
                assertTrue(hazelcastClient.removeLifecycleListener(&lifecycleListener), "Listener could not removed");
            }

            void ClusterTest::testBehaviourWhenClusterNotFound() {
                ClientConfig clientConfig;
                try {
                    HazelcastClient hazelcastClient(clientConfig);
                    assertTrue(false);
                } catch (exception::IllegalStateException&) {

                }
            }
        }
    }
}


