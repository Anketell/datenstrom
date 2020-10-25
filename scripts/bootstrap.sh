#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

echo ${root}

mkdir -p ${root}/debug

pushd ${root}/debug >/dev/null
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DDS_CSV=YES \
      -DDS_DB=YES \
      -DDS_FIREBIRD=YES \
      -DDS_MYSQL=YES \
      -DDS_SQLITE=YES ..
popd

mkdir -p ${root}/release

pushd ${root}/release >/dev/null
cmake -DCMAKE_BUILD_TYPE=Release \
      -DDS_CSV=YES \
      -DDS_DB=YES \
      -DDS_FIREBIRD=YES \
      -DDS_MYSQL=YES \
      -DDS_SQLITE=YES ..
popd

mkdir -p ${root}/coverage

pushd ${root}/coverage >/dev/null
cmake -DCMAKE_BUILD_TYPE=Coverage \
      -DDS_CSV=YES \
      -DDS_DB=YES \
      -DDS_FIREBIRD=YES \
      -DDS_MYSQL=YES \
      -DDS_SQLITE=YES ..
popd >/dev/null
