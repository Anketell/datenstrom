set( TARGET ds )

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ds SHARED
   ds.cpp
)

add_subdirectory( test )
