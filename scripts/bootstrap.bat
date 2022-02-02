
@echo off

pushd %~dp0\..

if NOT EXIST build mkdir build

REM pushd third-party\googletest
REM mkdir build
REM cd build
REM "C:\Program Files\CMake\bin\cmake.exe" ..
REM popd

cd build
"C:\Program Files\CMake\bin\cmake.exe" -DDS_BIN=YES -DDS_CSV=YES -DDS_DB=YES -DDS_FIREBIRD=YES -DDS_SQLITE=YES -DDS_MSSQL=YES ..

popd