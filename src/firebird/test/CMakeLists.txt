#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_firebird )

set( SOURCE
   test_firebird_db.cpp
   test_firebird_list.cpp
   test_firebird_parameter.cpp
   test_firebird_transaction.cpp
   test_firebird_blob.cpp
   test_firebird_context.cpp
   firebird_test_data.cpp
)

set( HEADERS
   firebird_test_data.h
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   test_db_context
   ds_db
   test_model
   gtest
   gtest_main
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

add_library( firebird_test_module MODULE
   firebird_test_module.cpp
)

target_link_libraries( firebird_test_module
   sql_module_harness
   ds_util
)

if ( MSVC )

   target_link_options( firebird_sql_module  PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )
   target_link_options( firebird_test_module PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/win32/def/sql_module.def" )

   target_link_options( ${TARGET} PRIVATE "/WHOLEARCHIVE:test_db_context" )


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
   target_link_options( firebird_test_module PRIVATE "--entry=enroll_sql" )

   target_link_options( ${TARGET} PRIVATE "-Wl,-L${CMAKE_BINARY_DIR}/src/db/test;"
                                          "-Wl,--whole-archive;"
                                          "-Wl,-ltest_db_context;"
                                          "-Wl,--no-whole-archive" )

endif( MSVC )
