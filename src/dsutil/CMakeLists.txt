#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_util )

if ( WIN32 )

   set( SOURCE
      ../../win32/dsutil/env.cpp
      ../../win32/dsutil/filesys.cpp
      ../../win32/dsutil/module.cpp
      ../../win32/dsutil/string.cpp
      ../../win32/dsutil/time.cpp
      parameter.cpp
   )

   set( HEADERS
      ../../win32/dsutil/env.h
      ../../win32/dsutil/filesys.h
      ../../win32/dsutil/module.h
      ../../win32/dsutil/string.h
      ../../win32/dsutil/time.h
      parameter.h
   )

else ( win32 )

   set( SOURCE
      parameter.cpp
   )

   set( HEADERS
      parameter.h
   )

endif ( WIN32 )

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)


if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
else ( WIN32 )
   target_link_libraries( ${TARGET} util )
endif( WIN32 )

add_subdirectory( test )
