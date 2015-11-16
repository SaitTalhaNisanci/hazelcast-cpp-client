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
// Created by sancar koyunlu on 01/10/14.
//

#include "hazelcast/client/proxy/TransactionalMapImpl.h"

#include "hazelcast/client/spi/ClusterService.h"

// Includes for parameters classes
#include "hazelcast/client/protocol/codec/TransactionalMapContainsKeyCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapGetCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapGetForUpdateCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapSizeCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapIsEmptyCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapPutCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapSetCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapPutIfAbsentCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapReplaceCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapReplaceIfSameCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapRemoveCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapDeleteCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapRemoveIfSameCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapKeySetCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapKeySetWithPredicateCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapValuesCodec.h"
#include "hazelcast/client/protocol/codec/TransactionalMapValuesWithPredicateCodec.h"

namespace hazelcast {
    namespace client {
        namespace proxy {
            TransactionalMapImpl::TransactionalMapImpl(const std::string& name, txn::TransactionProxy *transactionProxy)
            : TransactionalObject("hz:impl:mapService", name, transactionProxy) {

            }

            bool TransactionalMapImpl::containsKey(const serialization::pimpl::Data& key) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapContainsKeyCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key);

                return invokeAndGetResult<bool>(request);
            }

            serialization::pimpl::Data TransactionalMapImpl::get(const serialization::pimpl::Data& key) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapGetCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key);

                return *invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data> >(request);
            }

            int TransactionalMapImpl::size() {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapSizeCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId());

                return invokeAndGetResult<int>(request);
            }

            serialization::pimpl::Data TransactionalMapImpl::put(
                    const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {

                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapPutCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, value, getTimeoutInMilliseconds());

                return *invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data> >(request);

            }

            void TransactionalMapImpl::set(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapSetCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, value);

                invoke(request);
            }

            serialization::pimpl::Data TransactionalMapImpl::putIfAbsent(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapPutIfAbsentCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, value);

                return *invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data> >(request);
            }

            serialization::pimpl::Data TransactionalMapImpl::replace(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapReplaceCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, value);

                return *invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data> >(request);
            }

            bool TransactionalMapImpl::replace(const serialization::pimpl::Data& key, const serialization::pimpl::Data& oldValue, const serialization::pimpl::Data& newValue) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapReplaceIfSameCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, oldValue, newValue);

                return invokeAndGetResult<bool>(request);
            }

            serialization::pimpl::Data TransactionalMapImpl::remove(const serialization::pimpl::Data& key) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapRemoveCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key);

                return *invokeAndGetResult<std::auto_ptr<serialization::pimpl::Data> >(request);
            }

            void TransactionalMapImpl::deleteEntry(const serialization::pimpl::Data& key) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapDeleteCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key);

                invoke(request);
            }

            bool TransactionalMapImpl::remove(const serialization::pimpl::Data& key, const serialization::pimpl::Data& value) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapRemoveIfSameCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), key, value);

                return invokeAndGetResult<bool>(request);
            }

            std::auto_ptr<protocol::DataArray> TransactionalMapImpl::keySet() {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapKeySetCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId());

                return invokeAndGetResult<std::auto_ptr<protocol::DataArray> >(request);
            }

            std::auto_ptr<protocol::DataArray> TransactionalMapImpl::keySet(const std::string& predicate) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapKeySetWithPredicateCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), toData<std::string>(predicate));

                return invokeAndGetResult<std::auto_ptr<protocol::DataArray> >(request);
            }

            std::auto_ptr<protocol::DataArray> TransactionalMapImpl::values() {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapValuesCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId());

                return invokeAndGetResult<std::auto_ptr<protocol::DataArray> >(request);
            }

            std::auto_ptr<protocol::DataArray> TransactionalMapImpl::values(const std::string& predicate) {
                std::auto_ptr<protocol::ClientMessage> request =
                        protocol::codec::TransactionalMapValuesWithPredicateCodec::RequestParameters::encode(
                                getName(), getTransactionId(), util::getThreadId(), toData<std::string>(predicate));

                return invokeAndGetResult<std::auto_ptr<protocol::DataArray> >(request);
            }
        }
    }
}

