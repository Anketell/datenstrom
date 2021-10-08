;------------------------------------------------------------------------------
;
;  Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
;
;------------------------------------------------------------------------------

[Setup]
AppName=Datenstrom
AppPublisher=Lucid Systems Pty Ltd
AppVersion=0.4.0 β
CreateAppDir=yes
DefaultDirName={autopf}\datenstrom
OutputBaseFilename=Datenstrom_Setup
ChangesEnvironment=yes
ArchitecturesInstallIn64BitMode=x64

;------------------------------------------------------------------------------

[Dirs]
Name: "{app}\include\util";  Permissions: users-full
Name: "{app}\include\ds";  Permissions: users-full
Name: "{app}\include\csv"; Permissions: users-full
Name: "{app}\include\db";  Permissions: users-full
Name: "{app}\lib";         Permissions: users-full
Name: "{app}\bin";         Permissions: users-full
Name: "{app}\module";      Permissions: users-full

;------------------------------------------------------------------------------

[Files]
Source: "../../win32/util/env.h";        DestDir: "{app}/include/util"
Source: "../../win32/util/filesys.h";    DestDir: "{app}/include/util"
Source: "../../win32/util/module.h";     DestDir: "{app}/include/util"
Source: "../../win32/util/string.h";     DestDir: "{app}/include/util"
Source: "../../win32/util/time.h";       DestDir: "{app}/include/util"
Source: "../../src/ds/ds.h";             DestDir: "{app}/include/ds"
Source: "../../src/ds/factory.h";        DestDir: "{app}/include/ds"
Source: "../../src/csv/csv.h";           DestDir: "{app}/include/csv"
Source: "../../src/db/connect_params.h"; DestDir: "{app}/include/db"
Source: "../../src/db/connect_string.h"; DestDir: "{app}/include/db"
Source: "../../src/db/connection.h";     DestDir: "{app}/include/db"
Source: "../../src/db/factory_helper.h"; DestDir: "{app}/include/db"
Source: "../../src/db/factory.h";        DestDir: "{app}/include/db"
Source: "../../src/db/impl.h";           DestDir: "{app}/include/db"
Source: "../../src/db/result.h";         DestDir: "{app}/include/db"
Source: "../../src/db/statement.h";      DestDir: "{app}/include/db"
Source: "../../src/db/transaction.h";    DestDir: "{app}/include/db"
Source: "../../src/db/transactional.h";  DestDir: "{app}/include/db"

Source: "../../build/src/util/Release/util.dll";  DestDir: "{app}/bin"
Source: "../../build/src/util/Release/util.lib";  DestDir: "{app}/lib"
Source: "../../build/src/ds/Release/ds.dll";      DestDir: "{app}/bin"
Source: "../../build/src/ds/Release/ds.lib";      DestDir: "{app}/lib"
Source: "../../build/src/csv/Release/ds_csv.dll"; DestDir: "{app}/bin"
Source: "../../build/src/csv/Release/ds_csv.lib"; DestDir: "{app}/lib"
Source: "../../build/src/db/Release/ds_db.dll";   DestDir: "{app}/bin"
Source: "../../build/src/db/Release/ds_db.lib";   DestDir: "{app}/lib"

Source: "../../build/src/firebird/Release/ds_firebird_module.dll"; DestDir: "{app}/module"
Source: "../../build/src/sqlite/Release/ds_sqlite_module.dll";     DestDir: "{app}/module"
Source: "../../build/win32/mssql/Release/ds_mssql_module.dll";     DestDir: "{app}/module"

;------------------------------------------------------------------------------

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
    ValueType: expandsz; ValueName: "DS_MODULE_PATH"; ValueData: "{app}/module"

Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
    ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}/bin";       \
    Check: NeedsAddPath( ExpandConstant( '{app}/bin' ) )

;------------------------------------------------------------------------------

[Code]
function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  { look for the path with leading and trailing semicolon }
  { Pos() returns 0 if not found }
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;

{-----------------------------------------------------------------------------}

