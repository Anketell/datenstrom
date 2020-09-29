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
include_directories( "${source_dir}/googletest/include" )

set_target_properties( gtest PROPERTIES "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtestd.lib" )

set_target_properties( gtest_main PROPERTIES "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtest_maind.lib" )

link_directories( "${binary_dir}/lib/Debug" )
