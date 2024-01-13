#!/bin/env sh

BOOST_VERSION=1.83.0
BROKEN_FILE="include/boost/property_tree/ptree_serialization.hpp"

bpkg build libboost-property-tree --configure-only --yes
curl -sSO "https://raw.githubusercontent.com/boostorg/property_tree/develop/$BROKEN_FILE"
mv ptree_serialization.hpp "libboost-property-tree-$BOOST_VERSION/$BROKEN_FILE"
bpkg install libboost-property-tree
