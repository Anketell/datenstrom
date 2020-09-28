set( TARGET ds_csv )

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ${TARGET} SHARED
   istream.cpp
   ostream.cpp
)

target_link_libraries( ${TARGET}
   ds
)

add_subdirectory( test )
