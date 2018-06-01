cmake_minimum_required( VERSION 2.8.12 FATAL_ERROR )
project( datenstrom )

enable_testing()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
   include("cmake/coverage.cmake")
endif(CMAKE_BUILD_TYPE STREQUAL "Coverage")

add_compile_options(-std=c++14)

# include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/third-party" )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/src" )

add_subdirectory( src )
