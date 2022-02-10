@echo off

pushd %~dp0\..

rmdir /q/s build\image

mkdir build\image\include\dsutil
mkdir build\image\include\ds
mkdir build\image\include\csv
mkdir build\image\include\db
mkdir build\image\include\bin
mkdir build\image\lib
mkdir build\image\bin
mkdir build\image\module
mkdir build\image\def

cp -v win32\dsutil\endian.h                                          build\image\include\dsutil
cp -v win32\dsutil\env.h                                             build\image\include\dsutil
cp -v win32\dsutil\filesys.h                                         build\image\include\dsutil
cp -v win32\dsutil\module.h                                          build\image\include\dsutil
cp -v win32\dsutil\string.h                                          build\image\include\dsutil
cp -v win32\dsutil\time.h                                            build\image\include\dsutil

cp -v src\ds\ds.h                                                    build\image\include\ds
cp -v src\ds\factory.h                                               build\image\include\ds

cp -v src\csv\csv.h                                                  build\image\include\csv

cp -v src\db\connect_params.h                                        build\image\include\db
cp -v src\db\connect_string.h                                        build\image\include\db
cp -v src\db\connection.h                                            build\image\include\db
cp -v src\db\context.h                                               build\image\include\db
cp -v src\db\factory_helper.h                                        build\image\include\db
cp -v src\db\factory.h                                               build\image\include\db
cp -v src\db\impl.h                                                  build\image\include\db
cp -v src\db\result.h                                                build\image\include\db
cp -v src\db\sql_map.h                                               build\image\include\db
cp -v src\db\statement.h                                             build\image\include\db
cp -v src\db\sql_module_harness.h                                    build\image\include\db
cp -v src\db\transaction.h                                           build\image\include\db
cp -v src\db\transactional.h                                         build\image\include\db

cp -v src\bin\bin.h                                                  build\image\include\bin

cp -v win32\def\sql_module.def                                       build\image\def

cp -v build\win32\dsutil\Release\ds_util.dll                         build\image\bin
cp -v build\win32\dsutil\Release\ds_util.lib                         build\image\lib

cp -v build\src\ds\Release\ds.dll                                    build\image\bin
cp -v build\src\ds\Release\ds.lib                                    build\image\lib

cp -v build\src\csv\Release\ds_csv.dll                               build\image\bin
cp -v build\src\csv\Release\ds_csv.lib                               build\image\lib

cp -v build\src\db\Release\ds_db.dll                                 build\image\bin
cp -v build\src\db\Release\ds_db.lib                                 build\image\lib
cp -v build\src\db\Release\sql_module_harness.lib                    build\image\lib

cp -v build\src\bin\Release\ds_bin.dll                               build\image\bin
cp -v build\src\bin\Release\ds_bin.lib                               build\image\lib

cp -v build\src\firebird\Release\ds_firebird_module.dll              build\image\module
cp -v build\src\sqlite\Release\ds_sqlite_module.dll                  build\image\module
cp -v build\win32\mssql\Release\ds_mssql_module.dll                  build\image\module

popd
