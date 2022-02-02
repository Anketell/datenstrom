#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET util )

set( SOURCE
   endian.cpp
   env.cpp
   filesys.cpp
   module.cpp
   string.cpp
   time.cpp
)

set( HEADERS
   endian.h
   env.h
   filesys.h
   module.h
   string.h
   time.h
)

add_library( ${TARGET} STATIC
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   dl
)

set_property( TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON )
