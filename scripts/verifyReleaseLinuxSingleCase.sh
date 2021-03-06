#!/usr/bin/env bash

set -e #abort the script at first failure

HZ_INSTALL_DIR=$1
HZ_VERSION="3.10-SNAPSHOT"
HZ_BIT_VERSION=$2
HZ_LIB_TYPE=$3

echo "Verifying the release located at ${HZ_INSTALL_DIR} for ${HZ_LIB_TYPE} ${HZ_BIT_VERSION} bit library."

cd examples
rm -rf build
mkdir build
cd build

echo "Generating the solution files for compilation without TLS"
cmake .. -DHAZELCAST_INSTALL_DIR=${HZ_INSTALL_DIR} -DHZ_VERSION=${HZ_VERSION} -DHZ_BIT=${HZ_BIT_VERSION} -DHZ_LIB_TYPE=${HZ_LIB_TYPE}

make -j 4

# clean the directory for a fresh build
rm -rf *

echo "Generating the solution files for compilation with TLS support"
cmake .. -DHAZELCAST_INSTALL_DIR=${HZ_INSTALL_DIR} -DHZ_VERSION=${HZ_VERSION} -DHZ_BIT=${HZ_BIT_VERSION} -DHZ_LIB_TYPE=${HZ_LIB_TYPE} -DHZ_COMPILE_WITH_SSL=ON

make -j 4

echo "Verification of the release located at ${HZ_INSTALL_DIR} for version ${HZ_VERSION} ${HZ_LIB_TYPE} ${HZ_BIT_VERSION}-bit library is finished."

exit 0


