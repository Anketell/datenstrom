#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

add_subdirectory( dsutil )

if ( DS_DB )

   if ( DS_FIREBIRD )
      add_subdirectory( firebird )
   endif()

   if ( DS_SQLITE )
      add_subdirectory( sqlite )
   endif()

endif()
