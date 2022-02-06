#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_db )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement.cpp
   result.cpp
   transaction.cpp
   connect_string.cpp
   parameter.cpp
)

set( HEADERS
   enroll.h
   connection.h
   statement.h
   result.h
   transaction.h
   connect_string.h
   connect_params.h
   factory.h
   factory_helper.h
   impl.h
   transactional.h
   parameter.h
)

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds
   ds_util
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
