#!/usr/bin/bash
#------------------------------------------------------------------------------

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

echo ${root}

. ${root}/scripts/import_env.sh

#------------------------------------------------------------------------------

defines="
        -DDS_BIN=${DS_BIN} \
        -DDS_CSV=${DS_CSV} \
        -DDS_DB=${DS_DB} \
        -DDS_FIREBIRD=${DS_FIREBIRD} \
        -DDS_MYSQL=${DS_MYSQL} \
        -DDS_SQLITE=${DS_SQLITE} \
        -DDS_MSSQL=${DS_MSSQL} \
        -DDS_POSTGRESQL=${DS_POSTGRESQL} \
        -DDS_FIREBIRD_TEST=${DS_FIREBIRD_TEST} \
        -DDS_MYSQL_TEST=${DS_MYSQL_TEST} \
        -DDS_MSSQL_TEST=${DS_MSSQL_TEST} \
        -DDS_POSTGRESQL_TEST=${DS_POSTGRESQL_TEST} \
        -DDS_SQLITE_TEST=${DS_SQLITE_TEST}
"

#------------------------------------------------------------------------------

mkdir -p ${root}/debug

pushd ${root}/debug >/dev/null
cmake -DCMAKE_BUILD_TYPE=Debug ${defines} ..
popd >/dev/null

#------------------------------------------------------------------------------

mkdir -p ${root}/release

pushd ${root}/release >/dev/null
cmake -DCMAKE_BUILD_TYPE=Release ${defines} ..
popd >/dev/null

#------------------------------------------------------------------------------

mkdir -p ${root}/coverage

pushd ${root}/coverage >/dev/null
cmake -DCMAKE_BUILD_TYPE=Coverage ${defines} ..
popd >/dev/null

#------------------------------------------------------------------------------
