//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_TYPES_H
#define DS_FIREBIRD_TYPES_H

//-----------------------------------------------------------------------------

#include <firebird/ibase.h>
#include <firebird/error.h>
#include <vector>
#include <malloc.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

constexpr int status_vector_length = 20;

//-----------------------------------------------------------------------------

struct stmt_t
{
   isc_stmt_handle stmt   = 0;
   XSQLDA        * xsqlda = nullptr;
   char            type;

   ~stmt_t( void )
   {
      if ( stmt && ( xsqlda == nullptr || xsqlda->sqld != 0 ) )
      {
         ISC_STATUS status[ status_vector_length ];

         isc_dsql_free_statement( status, &stmt, DSQL_drop );

         check_status( "Fierbird drop statement", status );
      }

      if ( xsqlda )
      {
         for ( int i = 0; i < xsqlda->sqld; i++ )
         {
            free( xsqlda->sqlvar[ i ].sqldata );
            free( xsqlda->sqlvar[ i ].sqlind );
         }

         free( xsqlda );
      }
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
