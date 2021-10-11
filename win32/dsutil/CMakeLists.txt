#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET util )

set( SOURCE
   env.cpp
   filesys.cpp
   module.cpp
   string.cpp
   time.cpp
)

set( HEADERS
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
