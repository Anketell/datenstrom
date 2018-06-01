#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

echo ${root}

mkdir -p ${root}/debug

pushd ${root}/debug >/dev/null
cmake -DCMAKE_BUILD_TYPE=Debug ..
popd

# mkdir -p coverage

# pushd ${root}/coverage >/dev/null
# cmake -DCMAKE_BUILD_TYPE=Coverage ..
# popd >/dev/null

