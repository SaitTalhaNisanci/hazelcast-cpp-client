//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/map/DestroyRequest.h"
#include "PortableHook.h"
#include "PortableWriter.h"
#include "PortableReader.h"

namespace hazelcast {
    namespace client {
        namespace map {
            DestroyRequest::DestroyRequest(const std::string& name)
            :name(name) {

            };

            int DestroyRequest::getFactoryId() const {
                return PortableHook::F_ID;
            }

            int DestroyRequest::getClassId() const {
                return PortableHook::DESTROY;
            }

            void DestroyRequest::writePortable(serialization::PortableWriter& writer) const {
                writer.writeUTF("name", name);
            };

            void DestroyRequest::readPortable(serialization::PortableReader& reader) {
                name = reader.readUTF("name");
            };
        }
    }
}


