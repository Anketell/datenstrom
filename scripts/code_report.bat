@echo off

pushd %~dp0\..\build

set /P VERSION=<%~dp0\VERSION
set DS_MODULE_PATH=./x64/win32/mssql/test/Debug
set SQL_MODULE_PATH=./x64/win32/mssql/test/Debug

opencppcoverage ^
--modules ds_mssql_module.dll --modules ds_db.dll --modules ds.dll ^
--sources datenstrom\win32\mssql\* --sources datenstrom\src\db\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/win32/mssql/test/Debug/test_ds_mssql.exe

set DS_MODULE_PATH=./x64/src/firebird/test/Debug
set SQL_MODULE_PATH=./x64/src/firebird/test/Debug

opencppcoverage ^
--modules ds_firebird_module.dll --modules ds_db.dll --modules ds.dll ^
--input_coverage test_ds_mssql.cov ^
--sources datenstrom\src\firebird\* --sources datenstrom\src\db\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/firebird/test/Debug/test_ds_firebird.exe

set DS_MODULE_PATH=./x64/src/sqlite/test/Debug
set SQL_MODULE_PATH=./x64/src/sqlite/test/Debug

opencppcoverage ^
--modules ds_sqlite_module.dll --modules ds_db.dll --modules ds.dll ^
--input_coverage test_ds_firebird.cov ^
--sources datenstrom\src\sqlite\* --sources datenstrom\src\db\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/sqlite/test/Debug/test_ds_sqlite.exe

opencppcoverage ^
--modules ds_db.dll --modules ds.dll ^
--input_coverage test_ds_sqlite.cov ^
--sources datenstrom\src\db\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/db/test/Debug/test_ds_db.exe

opencppcoverage ^
--modules ds_bin.dll --modules ds.dll ^
--input_coverage test_ds_db.cov ^
--sources datenstrom\src\bin\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/bin/test/Debug/test_ds_bin.exe

opencppcoverage ^
--modules ds_csv.dll --modules ds.dll ^
--input_coverage test_ds_bin.cov ^
--sources datenstrom\src\csv\* --sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  x64/src/csv/test/Debug/test_ds_csv.exe

opencppcoverage ^
--modules ds_util.dll --modules test_ds_util.exe ^
--input_coverage test_ds_csv.cov ^
--sources dsutil\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/dsutil/test/Debug/test_ds_util.exe

opencppcoverage ^
--modules ds.dll --modules test_ds.exe ^
--input_coverage test_ds_util.cov ^
--sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
-- x64/src/ds/test/Debug/test_ds.exe

rd /s /q code_report

pryda_code_report.exe ^
-s test_ds.cov ^
-p "Lucid Datenstrom v%VERSION%" ^
-j .. ^
-r code_report

popd
