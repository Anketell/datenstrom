#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_mysql )

set( SOURCE
   test_mysql_db.cpp
   test_mysql_list.cpp
   test_mysql_parameter.cpp
   test_mysql_transaction.cpp
   test_mysql_blob.cpp
   test_mysql_context.cpp
   mysql_test_data.cpp
)

set( HEADERS
   mysql_test_data.h
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_dependencies( ${TARGET} ds_mysql_module )

add_test( ${TARGET} ${TARGET} )

add_library( mysql_sql_module MODULE
   mysql_sql_module.cpp
)

target_link_libraries( mysql_sql_module
   sql_module_harness
   ds_util
)

add_library( mysql_test_module MODULE
   mysql_test_module.cpp
)

target_link_libraries( mysql_test_module
   sql_module_harness
   ds_util
)

if ( MSVC )

   target_link_options( mysql_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )
   target_link_options( mysql_test_module PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

   target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_context" )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
      "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
      "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
      "${CMAKE_BINARY_DIR}/src/mysql/Debug/ds_mysql_module.dll"
   )

   foreach( dll ${DLL_LIST} )
      add_custom_command(TARGET ${TARGET} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
            "${dll}"
            "${CMAKE_CURRENT_BINARY_DIR}/Debug/"
      )
   endforeach( dll )

   set_target_properties( ${TARGET} PROPERTIES
      VS_DEBUGGER_ENVIRONMENT "DS_MODULE_PATH=${CMAKE_CURRENT_BINARY_DIR}/Debug/"
   )

else( MSVC )

   target_link_options( mysql_sql_module PRIVATE "--entry=enroll_sql" )
   target_link_options( mysql_test_module PRIVATE "--entry=enroll_sql" )

   target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                          "-Wl,--whole-archive;"
                                          "-Wl,-ltest_db_context;"
                                          "-Wl,--no-whole-archive" )

endif( MSVC )
