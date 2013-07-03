//
// Created by sancar koyunlu on 5/28/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "AddMembershipListenerRequest.h"
#include "ConstantSerializers.h"


namespace hazelcast {
    namespace client {
        namespace protocol {
            AddMembershipListenerRequest::AddMembershipListenerRequest() {

            };

            int AddMembershipListenerRequest::getFactoryId() const{
                return protocol::ProtocolConstants::DATA_FACTORY_ID;
            }

            int AddMembershipListenerRequest::getClassId() const {
                return protocol::ProtocolConstants::ADD_MS_LISTENER;
            }

            void AddMembershipListenerRequest::writeData(serialization::BufferedDataOutput& writer) {
            };

            void AddMembershipListenerRequest::readData(serialization::BufferedDataInput& reader) {
            };
        }
    }
}