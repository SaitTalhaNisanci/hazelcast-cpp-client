//
// Created by sancar koyunlu on 5/23/13.
// Copyright (c) 2013 hazelcast. All rights reserved.
#ifndef HAZELCAST_UNLOCK_REQUEST
#define HAZELCAST_UNLOCK_REQUEST

#include "Portable.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace serialization {
            class Data;
        }
        namespace map {
            class UnlockRequest : public Portable {
            public:
                UnlockRequest(const std::string& name, serialization::Data& key, int threadId);

                UnlockRequest(const std::string& name, serialization::Data& key, int threadId, bool force);

                int getFactoryId() const;

                int getClassId() const;

                void writePortable(serialization::PortableWriter& writer) const;

                void readPortable(serialization::PortableReader& reader);

            private:
                serialization::Data& key;
                std::string name;
                int threadId;
                bool force;
            };
        }
    }
}

#endif //HAZELCAST_MAP_PUT_REQUEST
