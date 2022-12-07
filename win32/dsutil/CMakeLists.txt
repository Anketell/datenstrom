#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_util )

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

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds_util_common
)

include( dll_funcs )
dll_def( TARGET ${TARGET} )
target_link_libraries( ${TARGET} Ws2_32 )
