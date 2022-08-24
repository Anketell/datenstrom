@echo off

set DS_MODULE_PATH=./build/x64/win32/mssql/test/Debug
set SQL_MODULE_PATH=./build/x64/win32/mssql/test/Debug

opencppcoverage ^
--modules ds_mssql_module.dll ^
--sources datenstrom\win32\mssql\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/win32/mssql/test/Debug/test_ds_mssql.exe

set DS_MODULE_PATH=./build/x64/src/firebird/test/Debug
set SQL_MODULE_PATH=./build/x64/src/firebird/test/Debug

opencppcoverage ^
--modules ds_firebird_module.dll ^
--input_coverage test_ds_mssql.cov ^
--sources datenstrom\src\firebird\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/firebird/test/Debug/test_ds_firebird.exe

set DS_MODULE_PATH=./build/x64/src/sqlite/test/Debug
set SQL_MODULE_PATH=./build/x64/src/sqlite/test/Debug

opencppcoverage ^
--modules ds_sqlite_module.dll ^
--input_coverage test_ds_firebird.cov ^
--sources datenstrom\src\sqlite\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/sqlite/test/Debug/test_ds_sqlite.exe

opencppcoverage ^
--modules ds_db.dll ^
--input_coverage test_ds_sqlite.cov ^
--sources datenstrom\src\db\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/db/test/Debug/test_ds_db.exe

opencppcoverage ^
--modules ds_bin.dll ^
--input_coverage test_ds_db.cov ^
--sources datenstrom\src\bin\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/bin/test/Debug/test_ds_bin.exe

opencppcoverage ^
--modules ds_csv.dll ^
--input_coverage test_ds_bin.cov ^
--sources datenstrom\src\csv\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/csv/test/Debug/test_ds_csv.exe

opencppcoverage ^
--modules ds_util.dll ^
--input_coverage test_ds_csv.cov ^
--sources datenstrom\win32\dsutil\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/dsutil/test/Debug/test_ds_util.exe

opencppcoverage ^
--modules ds.dll ^
--input_coverage test_ds_util.cov ^
--sources datenstrom\src\ds\* ^
--excluded_sources \test* ^
--excluded_sources \third-party\* ^
--export_type binary ^
--  build/x64/src/ds/test/Debug/test_ds.exe

..\coverage_display_test\build\src\code_report\RelWithDebInfo\code_report.exe ^
-s test_ds.cov ^
-p Lucid.Datenstrom ^
-j %~dp0/.. ^
-r output ^
-t ..\coverage_display_test\template
-w ..\coverage_display_test\www
