#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

include( ExternalProject )

ExternalProject_Add(
    googletest
    PREFIX "googletest"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/third-party/googletest"
    INSTALL_COMMAND ""
    DOWNLOAD_COMMAND ""
    LOG_CONFIGURE ON
    LOG_BUILD ON
    CMAKE_ARGS "-Dgtest_force_shared_crt=ON -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
)

add_library( gtest IMPORTED STATIC GLOBAL )
add_library( gtest_main IMPORTED STATIC GLOBAL )

ExternalProject_Get_Property( googletest source_dir binary_dir )

set_target_properties( gtest PROPERTIES "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtest.lib" )

set_target_properties( gtest_main PROPERTIES "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtest_main.lib" )

link_directories( "${binary_dir}/lib/Debug" )

include_directories( "${source_dir}/googletest/include" )

# set( gtest_link_dir ${binary_dir}/lib/$<CONFIG> )

# message( NOTICE ${gtest_link_dir} )

# set( gtest_target ${gtest_link_dir}/$<$<CONFIG:Debug>:gtestd.lib>$<$<CONFIG:Release>:gtest.lib> )

# set( gtest_main_target ${gtest_link_dir}/$<$<CONFIG:Debug>:gtest_maind.lib>$<$<CONFIG:Release>:gtest_main.lib> )

# set_target_properties( gtest PROPERTIES "IMPORTED_LOCATION" ${gtest_target} )

# set_target_properties( gtest_main PROPERTIES "IMPORTED_LOCATION" ${gtest_main_target} )

# link_directories( ${gtest_link_dir} )

