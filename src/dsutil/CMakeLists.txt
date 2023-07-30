#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_util_common )

set( SOURCE
   connect_string.cpp
   timestamp.cpp
   url_encode.cpp
)

set( HEADERS
   connect_params.h
   connect_string.h
   timestamp.h
   url_encode.h
)

add_library( ${TARGET} STATIC
   ${SOURCE}
   ${HEADERS}
)

if ( UNIX )

   target_compile_options( ${TARGET} PRIVATE "-fPIC" )

endif( UNIX )

add_subdirectory( test )
