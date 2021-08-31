#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds )

set( SOURCE
   ds.cpp
)

set( HEADERS
   ds.h
   factory.h
)

add_library( ds SHARED
   ${SOURCE}
   ${HEADERS}
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
