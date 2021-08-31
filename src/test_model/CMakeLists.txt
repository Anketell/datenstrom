#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( SOURCE
   object.cpp
   object_serialise.cpp
)

set( HEADERS
   object.h
   object_serialise.h
)

add_library( test_model STATIC
   ${SOURCE}
   ${HEADERS}
)
