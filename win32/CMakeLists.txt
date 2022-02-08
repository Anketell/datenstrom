#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

add_subdirectory( dsutil )

if ( DS_DB )

   if ( DS_MSSQL )
      add_subdirectory( mssql )
   endif( DS_MSSQL )

endif ( DS_DB )