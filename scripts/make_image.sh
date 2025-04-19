#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null

rm -rf ./release/image

mkdir -p ./release/image/usr/include/datenstrom/{dsutil,ds,csv,db,bin}
mkdir -p ./release/image/usr/lib64/datenstrom
mkdir -p ./release/image/usr/share/datenstrom/module

cp ./src/dsutil/url_encode.h                         ./release/image/usr/include/datenstrom/dsutil
cp ./src/dsutil/connect_params.h                     ./release/image/usr/include/datenstrom/dsutil
cp ./src/dsutil/connect_string.h                     ./release/image/usr/include/datenstrom/dsutil
cp ./src/dsutil/timestamp.h                          ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/endian.h                           ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/env.h                              ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/filesys.h                          ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/module.h                           ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/string.h                           ./release/image/usr/include/datenstrom/dsutil
cp ./linux/dsutil/time.h                             ./release/image/usr/include/datenstrom/dsutil

cp ./src/ds/ds.h                                     ./release/image/usr/include/datenstrom/ds
cp ./src/ds/factory.h                                ./release/image/usr/include/datenstrom/ds

cp ./src/csv/csv.h                                   ./release/image/usr/include/datenstrom/csv

cp ./src/db/connection.h                             ./release/image/usr/include/datenstrom/db
cp ./src/db/context.h                                ./release/image/usr/include/datenstrom/db
cp ./src/db/factory_helper.h                         ./release/image/usr/include/datenstrom/db
cp ./src/db/factory.h                                ./release/image/usr/include/datenstrom/db
cp ./src/db/impl.h                                   ./release/image/usr/include/datenstrom/db
cp ./src/db/rowset.h                                 ./release/image/usr/include/datenstrom/db
cp ./src/db/sql_map.h                                ./release/image/usr/include/datenstrom/db
cp ./src/db/statement.h                              ./release/image/usr/include/datenstrom/db
cp ./src/db/sql_module_harness.h                     ./release/image/usr/include/datenstrom/db
cp ./src/db/transaction.h                            ./release/image/usr/include/datenstrom/db
cp ./src/db/transactional.h                          ./release/image/usr/include/datenstrom/db

cp ./src/bin/bin.h                                   ./release/image/usr/include/datenstrom/bin

cp ./release/linux/dsutil/libds_util.so              ./release/image/usr/lib64/datenstrom
cp ./release/src/ds/libds.so                         ./release/image/usr/lib64/datenstrom
cp ./release/src/csv/libds_csv.so                    ./release/image/usr/lib64/datenstrom
cp ./release/src/db/libds_db.so                      ./release/image/usr/lib64/datenstrom
cp ./release/src/db/libsql_module_harness.a          ./release/image/usr/lib64/datenstrom
cp ./release/src/bin/libds_bin.so                    ./release/image/usr/lib64/datenstrom

cp ./release/src/firebird/libds_firebird_module.so   ./release/image/usr/share/datenstrom/module
cp ./release/src/mysql/libds_mysql_module.so         ./release/image/usr/share/datenstrom/module
cp ./release/src/sqlite/libds_sqlite_module.so       ./release/image/usr/share/datenstrom/module

popd >/dev/null
