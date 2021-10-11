#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_sqlite )

set( SOURCE
   test_sqlite_db.cpp
   test_sqlite_transaction.cpp
   test_sqlite_parameter.cpp
   test_sqlite_list.cpp
   test_sqlite_blob.cpp
   sqlite_test_data.cpp
)

set( HEADERS
   sqlite_test_data.h
)

add_executable( ${TARGET}
   ${SOURCE}
   ${HEADERS}
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )

if ( MSVC )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/src/dsutil/Debug/ds_util.dll"
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

   set_target_properties( ${TARGET} PROPERTIES
      VS_DEBUGGER_ENVIRONMENT "DS_MODULE_PATH=${CMAKE_CURRENT_BINARY_DIR}/Debug/"
   )

endif( MSVC )
