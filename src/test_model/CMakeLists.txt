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

if ( MSVC )

   set_target_properties( test_model
      PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE 1
   )

endif()