@echo off

pushd %~dp0\..

call scripts\import_env.bat 

if %errorlevel% NEQ 0 exit /b %errorlevel%

setlocal enabledelayedexpansion

set defines=-DDS_BIN=%DS_BIN% ^
        -DDS_CSV=%DS_CSV% ^
        -DDS_DB=%DS_DB% ^
        -DDS_FIREBIRD=%DS_FIREBIRD% ^
        -DDS_MYSQL=%DS_MYSQL% ^
        -DDS_SQLITE=%DS_SQLITE% ^
        -DDS_MSSQL=%DS_MSSQL% ^
        -DDS_POSTGRESQL=%DS_POSTGRESQL% ^
        -DDS_FIREBIRD_TEST=%DS_FIREBIRD_TEST% ^
        -DDS_MYSQL_TEST=%DS_MYSQL_TEST% ^
        -DDS_MSSQL_TEST=%DS_MSSQL_TEST% ^
        -DDS_POSTGRESQL_TEST=%DS_POSTGRESQL_TEST% ^
        -DDS_SQLITE_TEST=%DS_SQLITE_TEST%

if NOT EXIST build\x64 mkdir build\x64

"C:\Program Files\CMake\bin\cmake.exe" -A x64 %defines% -S . -B build\x64

if NOT EXIST build\x86 mkdir build\x86

"C:\Program Files\CMake\bin\cmake.exe" -A Win32 %defines% -S . -B build\x86

endlocal

popd
