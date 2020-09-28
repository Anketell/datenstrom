# We need thread support
# find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Build gtest from existing sources
ExternalProject_Add(
googletest
PREFIX "googletest" # But use prefix for compute other dirs
SOURCE_DIR "${CMAKE_SOURCE_DIR}/third-party/googletest" # Use specific source dir
INSTALL_COMMAND ""
DOWNLOAD_COMMAND "" # No download required
LOG_CONFIGURE ON
LOG_BUILD ON
CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
)

# gtest should be build after being downloaded
add_library(gtest IMPORTED STATIC GLOBAL)
add_library(gtest_main IMPORTED STATIC GLOBAL)

ExternalProject_Get_Property(googletest source_dir binary_dir)
include_directories("${source_dir}/googletest/include")

set_target_properties(gtest PROPERTIES
"IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtestd.lib"
# "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

set_target_properties(gtest_main PROPERTIES
"IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtest_maind.lib"
# "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

link_directories( "${binary_dir}/lib/Debug" )
