#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET util )

set( SOURCE
   parameter.cpp
)

set( HEADERS
   parameter.h
)

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)

if ( UNIX )
   target_link_libraries( ${TARGET} linuxutil )
#   set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ( UNIX )

if ( WIN32 )
   target_link_libraries( ${TARGET} win32util )
endif ( WIN32 )

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
