cmake_minimum_required( VERSION 2.8.12 FATAL_ERROR )
project( datenstrom )

enable_testing()

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

if ( WIN32 )
   include( googletest )
endif ( WIN32 )

if ( UNIX )
   set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )

   if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
      include("coverage")
   endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

   add_compile_options(-std=c++14)
endif ( UNIX )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/src" )

if ( WIN32 )
   include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/win32" )
   add_subdirectory( win32 )
endif ( WIN32 )

if ( UNIX )
   include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/linux" )
   add_subdirectory( linux )
endif ( UNIX )

add_subdirectory( src )
