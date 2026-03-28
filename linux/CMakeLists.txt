#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

if ( DS_DB )

   if ( DS_MSSQL )
      add_subdirectory( mssql )
   endif()

endif()

add_subdirectory( dsutil )
