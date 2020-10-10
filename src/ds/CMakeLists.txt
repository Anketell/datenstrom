set( TARGET ds )

# set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ds SHARED
   ds.cpp
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
