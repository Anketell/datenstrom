#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds )

add_executable( ${TARGET}
   test_ds.cpp
   test_factory.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )

if ( MSVC )

   set_target_properties( ${TARGET}
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

   add_custom_command(TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
         "${CMAKE_BINARY_DIR}/src/ds/Debug/ds.dll"
         "${CMAKE_CURRENT_BINARY_DIR}/Debug/ds.dll"
   )

endif( MSVC )
