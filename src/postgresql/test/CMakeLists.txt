#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_postgresql )

set( SOURCE
   test_postgresql.cpp
)

set( HEADERS
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   test_db_module
   ds_db
   test_model
   gtest
   gtest_main
)

target_compile_definitions( ${TARGET} PUBLIC
   DS_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../"
   SQL_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}"
   POSTGRESQL_TEST="${DS_POSTGRESQL_TEST}"
)

add_dependencies( ${TARGET} ds_postgresql_module )

add_test( ${TARGET} ${TARGET} )

add_library( postgresql_sql_module MODULE
   postgresql_sql_module.cpp
)

target_link_libraries( postgresql_sql_module
   sql_module_harness
   ds_util
)

if ( MSVC )

   set_target_properties( ${TARGET}
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   set_target_properties( postgresql_sql_module
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   target_link_options( postgresql_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

   target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_module" )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
      "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
      "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
      "${CMAKE_BINARY_DIR}/src/postgresql/Debug/ds_postgresql_module.dll"
   )

   foreach( dll ${DLL_LIST} )
      add_custom_command(TARGET ${TARGET} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
            "${dll}"
            "${CMAKE_CURRENT_BINARY_DIR}/Debug/"
      )
   endforeach( dll )

else( MSVC )

   set_target_properties( postgresql_sql_module PROPERTIES PREFIX "" )

   target_link_options( postgresql_sql_module PRIVATE "--entry=enroll_sql" )

   target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                          "-Wl,--whole-archive;"
                                          "-Wl,-ltest_db_module;"
                                          "-Wl,--no-whole-archive" )

endif( MSVC )
