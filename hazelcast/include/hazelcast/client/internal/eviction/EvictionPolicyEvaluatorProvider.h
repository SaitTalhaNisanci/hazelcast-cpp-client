/*
 * Copyright (c) 2008-2016, Hazelcast, Inc. All Rights Reserved.
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
#ifndef HAZELCAST_CLIENT_INTERNAL_EVICTION_EVICTIONPOLICYEVALUATORPROVIDER_H_
#define HAZELCAST_CLIENT_INTERNAL_EVICTION_EVICTIONPOLICYEVALUATORPROVIDER_H_

#include <assert.h>
#include <boost/shared_ptr.hpp>

#include "hazelcast/client/exception/IllegalArgumentException.h"
#include "hazelcast/client/internal/eviction/impl/comparator/LRUEvictionPolicyComparator.h"
#include "hazelcast/client/internal/eviction/impl/comparator/LFUEvictionPolicyComparator.h"
#include "hazelcast/client/internal/eviction/impl/comparator/RandomEvictionPolicyComparator.h"
#include "hazelcast/client/internal/eviction/EvictionConfiguration.h"
#include "hazelcast/client/internal/eviction/EvictionPolicyEvaluator.h"
#include "hazelcast/client/internal/eviction/impl/evaluator/DefaultEvictionPolicyEvaluator.h"

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4251) //for dll export
#endif

namespace hazelcast {
    namespace client {
        namespace internal {
            namespace eviction {
                /**
                 * Provider to get any kind ({@link EvictionPolicyType}) of {@link EvictionPolicyEvaluator}.
                 */
                class EvictionPolicyEvaluatorProvider {
                public:
                    /**
                     * Gets the {@link EvictionPolicyEvaluator} implementation specified with {@code evictionPolicy}.
                     *
                     * @param E is a type that extends Evictable
                     * @param evictionConfig {@link EvictionConfiguration} for requested {@link EvictionPolicyEvaluator} implementation
                     * @return the requested {@link EvictionPolicyEvaluator} implementation
                     */
                    template <typename A, typename E>
                static std::auto_ptr<EvictionPolicyEvaluator<A, E> > getEvictionPolicyEvaluator(
                            const boost::shared_ptr<EvictionConfiguration<A, E> > &evictionConfig) {
                        if (evictionConfig.get() == NULL) {
                            return std::auto_ptr<EvictionPolicyEvaluator<A, E> >();
                        }

                        boost::shared_ptr<EvictionPolicyComparator<A, E> > evictionPolicyComparator;

                        const boost::shared_ptr<EvictionPolicyComparator<A, E> > &comparator = evictionConfig->getComparator();
                        if (comparator.get() != NULL) {
                            evictionPolicyComparator = comparator;
                        } else {
                            EvictionPolicyType evictionPolicyType = evictionConfig->getEvictionPolicyType();
                            evictionPolicyComparator = createEvictionPolicyComparator(evictionPolicyType);
                        }

                        return std::auto_ptr<EvictionPolicyEvaluator<A, E> >(new impl::evaluator::DefaultEvictionPolicyEvaluator<A, E>(evictionPolicyComparator));
                    }

                private:
                    template <typename A, typename E>
                    static boost::shared_ptr<EvictionPolicyComparator<A, E> > createEvictionPolicyComparator(EvictionPolicyType evictionPolicyType) {
                        switch (evictionPolicyType) {
                            case LRU:
                                return boost::shared_ptr<EvictionPolicyComparator<A, E> >(new impl::comparator::LRUEvictionPolicyComparator());
                            case LFU:
                                return boost::shared_ptr<EvictionPolicyComparator<A, E> >(new impl::comparator::LFUEvictionPolicyComparator());
                            case RANDOM:
                                return boost::shared_ptr<EvictionPolicyComparator<A, E> >(new impl::comparator::RandomEvictionPolicyComparator());
                            case NONE:
                                return boost::shared_ptr<EvictionPolicyComparator<A, E> >();
                            default:
                                throw exception::IllegalArgumentException(std::string("Unsupported eviction policy type: ") + evictionPolicyType);
                        }
                    }

                    //Non-constructable class
                    EvictionPolicyEvaluatorProvider();
                    ~EvictionPolicyEvaluatorProvider();
                };

            }
        }
    }
};

#if  defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(pop)
#endif 

#endif /* HAZELCAST_CLIENT_INTERNAL_EVICTION_EVICTIONPOLICYEVALUATORPROVIDER_H_ */