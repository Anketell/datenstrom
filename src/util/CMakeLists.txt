set( TARGET util )

add_library( ${TARGET} STATIC
   parameter.cpp
)

if ( UNIX )
   target_link_libraries( ${TARGET} linuxutil )
   set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ( UNIX )

if ( WIN32 )
   target_link_libraries( ${TARGET} win32util )
endif ( WIN32 )

