include_directories( BEFORE, ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( ds )
add_subdirectory( csv )
add_subdirectory( db )
add_subdirectory( firebird )
add_subdirectory( sqlite )
add_subdirectory( mysql )
add_subdirectory( test_model )
