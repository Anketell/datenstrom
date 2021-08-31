add_subdirectory( util )

if ( DS_DB )

   if ( DS_MSSQL )
      add_subdirectory( mssql )
   endif( DS_MSSQL )

endif ( DS_DB )