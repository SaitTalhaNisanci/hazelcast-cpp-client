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
 * ClientMessageBuilder.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: ihsan
 */



#include "hazelcast/client/protocol/ClientMessageBuilder.h"
#include "hazelcast/client/protocol/IMessageHandler.h"
#include "hazelcast/util/ByteBuffer.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            ClientMessageBuilder::ClientMessageBuilder(IMessageHandler *service, connection::Connection &connection)
            : messageHandler(service), connection(connection) {
            }

            ClientMessageBuilder::~ClientMessageBuilder() {
                for (MessageMap::iterator it = partialMessages.begin(); it != partialMessages.end(); ++it) {
                    delete it->second;
                }
            }

            bool ClientMessageBuilder::onData(util::ByteBuffer &buffer) {
                bool isFrameComplete = true;
                do {
                    if (NULL == message.get()) {
                        if (buffer.remaining() >= ClientMessage::HEADER_SIZE) {
                            wrapperMessage.wrapForDecode((byte *) buffer.ix(), (int32_t) buffer.remaining(), false);
                            int32_t frameLen = wrapperMessage.getFrameLength();
                            message = std::auto_ptr<ClientMessage>(new ClientMessage(frameLen));
                        } else {
                            isFrameComplete = false; // not enough bytes to for the frame header
                        }
                    }

                    if (isFrameComplete) {
                        isFrameComplete = message->fillMessageFrom(buffer);
                    }

                    if (isFrameComplete) {
                        if (message->isFlagSet(ClientMessage::BEGIN_AND_END_FLAGS)) {
                            //MESSAGE IS COMPLETE HERE
                            if (messageHandler) {
                                messageHandler->handleMessage(connection, message);
                            }
                        } else {
                            if (message->isFlagSet(ClientMessage::BEGIN_FLAG)) {
                                // put the message into the partial messages list
                                addToPartialMessages(message);
                            } else if (message->isFlagSet(ClientMessage::END_FLAG)) {
                                // This is the intermediate frame. Append at the previous message buffer
                                appendExistingPartialMessage(message);
                            }
                        }
                    }
                } while (isFrameComplete); // end of while

                return isFrameComplete;
            }

            void ClientMessageBuilder::addToPartialMessages(std::auto_ptr<ClientMessage> message) {
                uint32_t id = message->getCorrelationId();
                partialMessages[id] = message.release();
            }

            bool ClientMessageBuilder::appendExistingPartialMessage(std::auto_ptr<ClientMessage> message) {
                bool result = false;

                MessageMap::iterator foundItemIter = partialMessages.find(message->getCorrelationId());
                if (partialMessages.end() != foundItemIter) {
                    foundItemIter->second->append(message.get());
                    if (message->isFlagSet(ClientMessage::END_FLAG)) {
                        // remove from message from map

                        std::auto_ptr<ClientMessage> foundMessage(foundItemIter->second);

                        partialMessages.erase(foundItemIter, foundItemIter);

                        if (messageHandler) {
                            messageHandler->handleMessage(connection, foundMessage);
                        }

                        result = true;
                    }
                } else {
                    // TODO: Print an error message and throw exception or something
                    assert(0);
                }

                return result;
            }
        }
    }
}