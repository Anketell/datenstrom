
@echo off

setlocal enabledelayedexpansion

pushd %~dp0\..

set env=scripts\.env

for /f "usebackq tokens=* delims=" %%A in ("%env%") do (

    set "line=%%A"
   
    echo !line!

    if "!line!"=="" (
        continue
    )

    if NOT "!line:~0,1!"=="#" (
        echo Setting "%%A"
        set "%%A"
    )
)

set defines=-DDS_BIN=!DS_BIN! ^
        -DDS_CSV=!DS_CSV! ^
        -DDS_DB=!DS_DB! ^
        -DDS_FIREBIRD=!DS_FIREBIRD! ^
        -DDS_MYSQL=!DS_MYSQL! ^
        -DDS_SQLITE=!DS_SQLITE! ^
        -DDS_MSSQL=!DS_MSSQL! ^
        -DDS_POSTGRESQL=!DS_POSTGRESQL! ^
        -DDS_FIREBIRD_TEST=!DS_FIREBIRD_TEST! ^
        -DDS_MYSQL_TEST=!DS_MYSQL_TEST! ^
        -DDS_MSSQL_TEST=!DS_MSSQL_TEST! ^
        -DDS_POSTGRESQL_TEST=!DS_POSTGRESQL_TEST! ^
        -DDS_SQLITE_TEST=!DS_SQLITE_TEST!


echo !defines!

if NOT EXIST build\x64 mkdir build\x64

"C:\Program Files\CMake\bin\cmake.exe" -A x64 !defines! -S . -B build\x64

if NOT EXIST build\x86 mkdir build\x86

"C:\Program Files\CMake\bin\cmake.exe" -A Win32 !defines! -S . -B build\x86
