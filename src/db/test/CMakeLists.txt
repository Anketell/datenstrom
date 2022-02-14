#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_db )

set( SOURCE
   test_db.cpp
   test_transaction.cpp
   test_list.cpp
   test_factory.cpp
   test_connect_string.cpp
   test_simple_result.cpp
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

add_library( test_db_context STATIC
   test_context.cpp
   test_context.h
)

if ( MSVC )

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
