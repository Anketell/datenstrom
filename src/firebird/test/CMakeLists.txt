#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_firebird )

set( SOURCE
   test_firebird.cpp
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

if( DEFINED ENV{DS_FIREBIRD_TEST} )
   set( FIREBIRD_TEST $ENV{DS_FIREBIRD_TEST} )
else()
   set( FIREBIRD_TEST "firebird://localhost/${TEMP}?username=sysdba&password=masterkey" )
endif()

target_compile_definitions( ${TARGET} PUBLIC  
   DS_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}/../"
   SQL_MODULE_PATH="${CMAKE_CURRENT_BINARY_DIR}"
   FIREBIRD_TEST="${FIREBIRD_TEST}"
)


add_dependencies( ${TARGET} ds_firebird_module )

add_test( ${TARGET} ${TARGET} )

add_library( firebird_sql_module MODULE
   firebird_sql_module.cpp
)

target_link_libraries( firebird_sql_module
   sql_module_harness
   ds_util
)

if ( MSVC )

   set_target_properties( ${TARGET}
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   set_target_properties( firebird_sql_module
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   target_link_options( firebird_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

   target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_module" )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
      "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
      "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
      "${CMAKE_BINARY_DIR}/src/firebird/Debug/ds_firebird_module.dll"
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

   target_link_options( firebird_sql_module PRIVATE "--entry=enroll_sql" )

   target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                          "-Wl,--whole-archive;"
                                          "-Wl,-ltest_db_module;"
                                          "-Wl,--no-whole-archive" )

endif( MSVC )
