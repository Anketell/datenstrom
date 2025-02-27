
@echo off

pushd %~dp0\..

if NOT EXIST build\x64 mkdir build\x64

REM pushd third-party\googletest
REM mkdir build
REM cd build
REM "C:\Program Files\CMake\bin\cmake.exe" ..
REM popd

"C:\Program Files\CMake\bin\cmake.exe" -A x64 -DDS_BIN=YES              ^
                                              -DDS_CSV=YES              ^
                                              -DDS_DB=YES               ^
                                              -DDS_FIREBIRD=YES         ^
                                              -DDS_SQLITE=YES           ^
                                              -DDS_MSSQL=YES -S . -B build\x64

if NOT EXIST build\x86 mkdir build\x86

"C:\Program Files\CMake\bin\cmake.exe" -A Win32 -DDS_BIN=YES              ^
                                                -DDS_CSV=YES              ^
                                                -DDS_DB=YES               ^
                                                -DDS_FIREBIRD=YES         ^
                                                -DDS_SQLITE=YES           ^
                                                -DDS_MSSQL=YES -S . -B build\x86
