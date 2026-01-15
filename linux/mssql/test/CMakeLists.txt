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

if( NOT DEFINED MSSQL_TEST )
   if( NOT DEFINED ENV{DS_MSSQL_TEST} )
      message( SEND_ERROR "DS_MSSQL_TEST is undefined - SQL Server test will not be built" )
      return()
   endif()
   set( MSSQL_TEST $ENV{DS_MSSQL_TEST} CACHE STRING "SQL Server test connect string" )
endif()

target_compile_definitions( ${TARGET} PUBLIC
   DS_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../"
   SQL_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}"
   MSSQL_TEST="${MSSQL_TEST}"
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


set_target_properties( mssql_sql_module PROPERTIES PREFIX "" )

target_link_options( mssql_sql_module PRIVATE "--entry=enroll_sql" )

target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                       "-Wl,--whole-archive;"
                                       "-Wl,-ltest_db_module;"
                                       "-Wl,--no-whole-archive" )
