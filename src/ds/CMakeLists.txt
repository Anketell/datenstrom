set( TARGET ds )

# set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ds SHARED
   ds.cpp
)

if ( WIN32 )
   set( DEF_FILE "${CMAKE_SOURCE_DIR}/win32/${TARGET}.def")
   target_link_options( ${TARGET} PRIVATE "/DEF:${DEF_FILE}" )
endif( WIN32 )

add_subdirectory( test )
