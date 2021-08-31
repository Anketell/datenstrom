#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_csv )

set( SOURCE
   istream.cpp
   ostream.cpp
)

set( HEADERS
   csv.h
)

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
