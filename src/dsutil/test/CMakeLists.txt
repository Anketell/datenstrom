#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET test_ds_util )

add_executable( ${TARGET}
   test_env.cpp
   test_filesys.cpp
   test_module.cpp
   test_string.cpp
   test_time.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds_util
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )

if ( MSVC )

   add_custom_command(TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
         "${CMAKE_BINARY_DIR}/src/dsutil/Debug/ds_util.dll"
         "${CMAKE_CURRENT_BINARY_DIR}/Debug/ds_util.dll"
   )

endif( MSVC )

add_library(  null_module MODULE
   null_module.cpp
)

if ( WIN32 )
   include( dll_funcs )
   module_def( TARGET null_module )
endif( WIN32 )
