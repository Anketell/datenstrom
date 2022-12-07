#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_util_common )

set( SOURCE
   connect_string.cpp
)

set( HEADERS
   connect_params.h
   connect_string.h
)

add_library( ${TARGET} STATIC
   ${SOURCE}
   ${HEADERS}
)

add_subdirectory( test )
