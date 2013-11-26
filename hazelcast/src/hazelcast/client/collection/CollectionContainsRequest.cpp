//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/collection/CollectionContainsRequest.h"
#include "hazelcast/client/serialization/Data.h"
#include "hazelcast/client/serialization/PortableWriter.h"
#include "hazelcast/client/collection/CollectionPortableHook.h"

namespace hazelcast {
    namespace client {
        namespace collection {
            CollectionContainsRequest::CollectionContainsRequest(const std::string& name, const std::vector<serialization::Data>& valueSet)
            : CollectionRequest(name)
            , valueSet(valueSet) {

            }

            void CollectionContainsRequest::writePortable(serialization::PortableWriter& writer) const {
                CollectionRequest::writePortable(writer);
                serialization::ObjectDataOutput & output = writer.getRawDataOutput();
                output.writeInt(valueSet.size());
                for (int i = 0; i < valueSet.size(); i++) {
                    valueSet[i].writeData(output);
                }
            }

            int CollectionContainsRequest::getClassId() const {
                return CollectionPortableHook::COLLECTION_CONTAINS;
            }


        }
    }
}