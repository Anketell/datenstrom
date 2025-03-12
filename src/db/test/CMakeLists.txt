#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_db )

set( SOURCE
   test_db.cpp
   test_transaction.cpp
   test_list.cpp
   test_factory.cpp
   test_simple_rowset.cpp
)

set( HEADERS
   test_db.h
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds
   ds_db
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )

set( TEST_MOD_SOURCE
   init_module_paths.cpp
   test_mod_connection.cpp
   test_mod_context.cpp
   test_mod_statement.cpp
   test_mod_rowset.cpp
   test_mod_blob.cpp
   test_mod_parameter.cpp
   test_mod_transaction.cpp
   test_mod_savepoint.cpp
   test_mod_list.cpp
)

set( TEST_MOD_HEADERS
   init_module_paths.h
   test_mod_config.h
   test_mod_connection.h
   test_mod_context.h
   test_mod_statement.h
   test_mod_rowset.h
   test_mod_blob.h
   test_mod_parameter.h
   test_mod_transaction.h
   test_mod_savepoint.h
   test_mod_list.h
)

add_library( test_db_module STATIC
   ${TEST_MOD_SOURCE}
   ${TEST_MOD_HEADERS}
)

if ( MSVC )

   set_target_properties( ${TARGET}
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   set_target_properties( test_db_module
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/win32/dsutil/Debug/ds_util.dll"
      "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
      "${CMAKE_BINARY_DIR}/src/db/Debug/ds_db.dll"
   )

   foreach( dll ${DLL_LIST} )
      add_custom_command(TARGET ${TARGET} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy
            "${dll}"
            "${CMAKE_CURRENT_BINARY_DIR}/Debug/"
      )
   endforeach( dll )

endif( MSVC )
