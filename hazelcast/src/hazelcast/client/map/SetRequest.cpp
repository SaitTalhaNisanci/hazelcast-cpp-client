//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/map/SetRequest.h"
#include "Data.h"
#include "PortableHook.h"
#include "PortableReader.h"
#include "PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace map {
            SetRequest::SetRequest(const std::string& name, serialization::Data& key, serialization::Data& value, int threadId, long ttl)
            :name(name)
            , key(key)
            , value(value)
            , threadId(threadId)
            , ttl(ttl) {

            };

            int SetRequest::getFactoryId() const {
                return PortableHook::F_ID;
            }

            int SetRequest::getClassId() const {
                return PortableHook::SET;
            }

            void SetRequest::writePortable(serialization::PortableWriter& writer) const {
                writer.writeUTF("n", name);
                writer.writeInt("t", threadId);
                writer.writeLong("ttl", ttl);
                serialization::ObjectDataOutput& out = writer.getRawDataOutput();
                key.writeData(out);
                value.writeData(out);
            };

            void SetRequest::readPortable(serialization::PortableReader& reader) {
                name = reader.readUTF("n");
                threadId = reader.readInt("t");
                ttl = reader.readLong("ttl");
                serialization::ObjectDataInput &in = reader.getRawDataInput();
                key.readData(in);
                value.readData(in);
            };
        }
    }
}

