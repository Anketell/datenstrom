#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

if ( DS_DB )

   set( DS_MODULE_PATH )

   if ( DS_FIREBIRD )
      string( APPEND DS_MODULE_PATH "${CMAKE_BINARY_DIR}/src/firebird/test/Debug;" )
   endif( DS_FIREBIRD )

   if ( DS_MYSQL )
      string( APPEND DS_MODULE_PATH "${CMAKE_BINARY_DIR}/src/mysql/test/Debug;" )
   endif( DS_MYSQL )

   if ( DS_SQLITE )
      string( APPEND DS_MODULE_PATH "${CMAKE_BINARY_DIR}/src/sqlite/test/Debug;" )
   endif( DS_SQLITE )

   if ( DS_MSSQL )
      string( APPEND DS_MODULE_PATH "${CMAKE_BINARY_DIR}/win32/mssql/test/Debug;" )
   endif( DS_MSSQL )

   file( WRITE "${CMAKE_BINARY_DIR}/.runsettings"
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<RunSettings>
<RunConfiguration>
   <EnvironmentVariables>
      <DS_MODULE_PATH>${DS_MODULE_PATH}</DS_MODULE_PATH>
   </EnvironmentVariables>
</RunConfiguration>
</RunSettings>"
   )

endif( DS_DB )

