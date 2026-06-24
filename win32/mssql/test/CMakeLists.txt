#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_mssql )

set( SOURCE
   test_mssql.cpp
)

set( HEADERS
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

add_dependencies( ${TARGET} googletest )

target_compile_definitions( ${TARGET} PUBLIC
   DS_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../"
   SQL_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}"
   MSSQL_TEST="${DS_MSSQL_TEST}"
)

target_link_libraries( ${TARGET}
   mssql_stmt_enum
   test_db_module
   ds_db
   test_model
   gtest
   gtest_main
)

add_dependencies( ${TARGET} ds_mssql_module )

add_test( ${TARGET} ${TARGET} )

add_library( mssql_sql_module MODULE
   mssql_sql_module.cpp
)

target_link_libraries( mssql_sql_module
   sql_module_harness
   ds_util
)

set_target_properties( ${TARGET}
   PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
)

set_target_properties( mssql_sql_module
   PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
)

target_link_options( mssql_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_module" )

set( DLL_LIST
   "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
   "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
   "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
   "${CMAKE_BINARY_DIR}/win32/mssql/Debug/ds_mssql_module.dll"
)

foreach( dll ${DLL_LIST} )
   add_custom_command(TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
         "${dll}"
         "${CMAKE_CURRENT_BINARY_DIR}/Debug/"
   )
endforeach( dll )
