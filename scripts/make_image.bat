@echo off

pushd %~dp0\..

rmdir /q/s build\image

mkdir build\image\include\dsutil
mkdir build\image\include\ds
mkdir build\image\include\csv
mkdir build\image\include\db
mkdir build\image\include\bin
mkdir build\image\lib\x64
mkdir build\image\lib\x86
mkdir build\image\bin\x64
mkdir build\image\bin\x86
mkdir build\image\module\x64
mkdir build\image\module\x86
mkdir build\image\def

xcopy /y /f src\dsutil\connect_params.h                                    build\image\include\dsutil
xcopy /y /f src\dsutil\connect_string.h                                    build\image\include\dsutil
xcopy /y /f win32\dsutil\endian.h                                          build\image\include\dsutil
xcopy /y /f win32\dsutil\env.h                                             build\image\include\dsutil
xcopy /y /f win32\dsutil\filesys.h                                         build\image\include\dsutil
xcopy /y /f win32\dsutil\module.h                                          build\image\include\dsutil
xcopy /y /f win32\dsutil\string.h                                          build\image\include\dsutil
xcopy /y /f win32\dsutil\time.h                                            build\image\include\dsutil

xcopy /y /f src\ds\ds.h                                                    build\image\include\ds
xcopy /y /f src\ds\factory.h                                               build\image\include\ds

xcopy /y /f src\csv\csv.h                                                  build\image\include\csv

xcopy /y /f src\db\connection.h                                            build\image\include\db
xcopy /y /f src\db\context.h                                               build\image\include\db
xcopy /y /f src\db\factory_helper.h                                        build\image\include\db
xcopy /y /f src\db\factory.h                                               build\image\include\db
xcopy /y /f src\db\impl.h                                                  build\image\include\db
xcopy /y /f src\db\result.h                                                build\image\include\db
xcopy /y /f src\db\sql_map.h                                               build\image\include\db
xcopy /y /f src\db\statement.h                                             build\image\include\db
xcopy /y /f src\db\sql_module_harness.h                                    build\image\include\db
xcopy /y /f src\db\transaction.h                                           build\image\include\db
xcopy /y /f src\db\transactional.h                                         build\image\include\db

xcopy /y /f src\bin\bin.h                                                  build\image\include\bin

xcopy /y /f win32\def\sql_module.def                                       build\image\def

xcopy /y /f build\x64\win32\dsutil\Release\ds_util.dll                     build\image\bin\x64
xcopy /y /f build\x64\win32\dsutil\Release\ds_util.lib                     build\image\lib\x64

xcopy /y /f build\x64\src\ds\Release\ds.dll                                build\image\bin\x64
xcopy /y /f build\x64\src\ds\Release\ds.lib                                build\image\lib\x64

xcopy /y /f build\x64\src\csv\Release\ds_csv.dll                           build\image\bin\x64
xcopy /y /f build\x64\src\csv\Release\ds_csv.lib                           build\image\lib\x64

xcopy /y /f build\x64\src\db\Release\ds_db.dll                             build\image\bin\x64
xcopy /y /f build\x64\src\db\Release\ds_db.lib                             build\image\lib\x64
xcopy /y /f build\x64\src\db\Release\sql_module_harness.lib                build\image\lib\x64

xcopy /y /f build\x64\src\bin\Release\ds_bin.dll                           build\image\bin\x64
xcopy /y /f build\x64\src\bin\Release\ds_bin.lib                           build\image\lib\x64

xcopy /y /f build\x64\src\firebird\Release\ds_firebird_module.dll          build\image\module\x64
xcopy /y /f build\x64\src\sqlite\Release\ds_sqlite_module.dll              build\image\module\x64
xcopy /y /f build\x64\win32\mssql\Release\ds_mssql_module.dll              build\image\module\x64

xcopy /y /f build\x86\win32\dsutil\Release\ds_util.dll                     build\image\bin\x86
xcopy /y /f build\x86\win32\dsutil\Release\ds_util.lib                     build\image\lib\x86

xcopy /y /f build\x86\src\ds\Release\ds.dll                                build\image\bin\x86
xcopy /y /f build\x86\src\ds\Release\ds.lib                                build\image\lib\x86

xcopy /y /f build\x86\src\csv\Release\ds_csv.dll                           build\image\bin\x86
xcopy /y /f build\x86\src\csv\Release\ds_csv.lib                           build\image\lib\x86

xcopy /y /f build\x86\src\db\Release\ds_db.dll                             build\image\bin\x86
xcopy /y /f build\x86\src\db\Release\ds_db.lib                             build\image\lib\x86
xcopy /y /f build\x86\src\db\Release\sql_module_harness.lib                build\image\lib\x86

xcopy /y /f build\x86\src\bin\Release\ds_bin.dll                           build\image\bin\x86
xcopy /y /f build\x86\src\bin\Release\ds_bin.lib                           build\image\lib\x86

xcopy /y /f build\x86\src\firebird\Release\ds_firebird_module.dll          build\image\module\x86
xcopy /y /f build\x86\src\sqlite\Release\ds_sqlite_module.dll              build\image\module\x86
xcopy /y /f build\x86\win32\mssql\Release\ds_mssql_module.dll              build\image\module\x86

popd
