set( TARGET ds_csv )

# set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ${TARGET} SHARED
   istream.cpp
   ostream.cpp
)

target_link_libraries( ${TARGET}
   ds
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
