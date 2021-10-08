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

add_test( ${TARGET} ${TARGET} )

if ( MSVC )

   set( DLL_LIST
      "${CMAKE_BINARY_DIR}/src/util/Debug/ds_util.dll"
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

endif( MSVC )
