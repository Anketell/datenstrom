#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_sqlite )

set( SOURCE
   test_sqlite.cpp
)

set( HEADERS
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   test_db_module
   ds_db
   test_model
   gtest
   gtest_main
)

if( DEFINED ENV{DS_SQLITE_TEST} )
   set( SQLITE_TEST $ENV{DS_SQLITE_TEST} )
else()
   set( SQLITE_TEST "sqlite:///${TEMP}" )
endif()

target_compile_definitions( ${TARGET} PUBLIC
   DS_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../"
   SQL_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}"
   SQLITE_TEST="${SQLITE_TEST}"
)

add_test( ${TARGET} ${TARGET} )

add_library( sqlite_sql_module MODULE
   sqlite_sql_module.cpp
)

target_link_libraries( sqlite_sql_module
   sql_module_harness
   ds_util
)

if ( MSVC )

   set_target_properties( ${TARGET}
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   set_target_properties( sqlite_sql_module
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   target_link_options( sqlite_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

   target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_module" )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
      "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
      "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
      "${CMAKE_BINARY_DIR}/src/sqlite/Debug/ds_sqlite_module.dll"
   )

   foreach( dll ${DLL_LIST} )
      add_custom_command(TARGET ${TARGET} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
            "${dll}"
            "${CMAKE_CURRENT_BINARY_DIR}/Debug/"
      )
   endforeach( dll )

else( MSVC )

   set_target_properties( sqlite_sql_module PROPERTIES PREFIX "" )

   target_link_options( sqlite_sql_module PRIVATE "--entry=enroll_sql" )

   target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                          "-Wl,--whole-archive;"
                                          "-Wl,-ltest_db_module;"
                                          "-Wl,--no-whole-archive" )

endif( MSVC )
