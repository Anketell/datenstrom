#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_mssql )

set( SOURCE
   test_mssql_db.cpp
   test_mssql_transaction.cpp
   test_mssql_parameter.cpp
   test_mssql_list.cpp
   mssql_test_data.cpp
)

set( HEADERS
   mssql_test_data.h
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

add_dependencies( ${TARGET} ds_mssql_module )

add_test( ${TARGET} ${TARGET} )

set( DLL_LIST
   "${CMAKE_BINARY_DIR}/src/dsutil/Debug/ds_util.dll"
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

set_target_properties( ${TARGET} PROPERTIES
   VS_DEBUGGER_ENVIRONMENT "DS_MODULE_PATH=${CMAKE_CURRENT_BINARY_DIR}/Debug/"
)
