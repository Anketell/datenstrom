#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

cmake_minimum_required( VERSION 3.13 FATAL_ERROR )

project( datenstrom )

enable_testing()

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set( CMAKE_CXX_STANDARD 17 )

if ( WIN32 )

   set( NUGET_REPO "nuget.org" )
   set( NUGET_OUTPUT_DIR "${CMAKE_BINARY_DIR}/third-party" )

   include( architecture )
   include( googletest )
   include( nuget )

   set( NUGET_VER 1,2.0 )

   nuget( NpgsqlBuilkCopy ${NUGET_VER} )

   add_compile_options( /Zc:__cplusplus )
   add_link_options( /Brepro )

   include( runsettings )

   cmake_path( CONVERT "$ENV{TEMP}" TO_CMAKE_PATH_LIST TEMP )

endif ( WIN32 )

if ( UNIX )
   set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )

   if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
      include("coverage")
   endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

   if(CMAKE_BUILD_TYPE STREQUAL "Profile")
      include("profile")
   endif(CMAKE_BUILD_TYPE STREQUAL "Profile")

   SET( TEMP "/tmp" )

   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_SKIP_RPATH TRUE)
   endif(CMAKE_BUILD_TYPE STREQUAL "Release")

endif ( UNIX )

include_directories( "${CMAKE_SOURCE_DIR}/src" )
include_directories( "${CMAKE_SOURCE_DIR}/third-party" )

if ( DS_DB AND DS_SQLITE )
   include_directories( "${CMAKE_SOURCE_DIR}/third-party/sqlite3/src" )
endif ( DS_DB AND DS_SQLITE )

if ( WIN32 )
   include_directories( BEFORE "${CMAKE_SOURCE_DIR}/win32" )
   add_subdirectory( win32 )
endif ( WIN32 )

if ( UNIX )
   include_directories( BEFORE "${CMAKE_SOURCE_DIR}/linux" )
   add_subdirectory( linux )
endif ( UNIX )

add_subdirectory( third-party )
add_subdirectory( src )
