//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <ibase.h>
#include <firebird/error.h>
#include <vector>
#include <malloc.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

constexpr int status_vector_length = 20;

//-----------------------------------------------------------------------------

struct stmt_t
{
   enum state_t { Preparing, Executed };

   isc_stmt_handle stmt   = 0;
   XSQLDA        * xsqlda = nullptr;
   int32_t         type;
   state_t         state;

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

ISC_DATE encode_sql_date( const char * );
ISC_TIME encode_sql_time( const char * );
ISC_TIMESTAMP encode_timestamp( const char * );
ISC_DATE encode_sql_unixdate( time_t );
ISC_TIME encode_sql_unixtime( time_t );
ISC_TIMESTAMP encode_unixtimestamp( time_t );
std::string decode_sql_date( ISC_DATE );
std::string decode_sql_time( ISC_TIME );
std::string decode_timestamp( ISC_TIMESTAMP );
time_t decode_sql_unixdate( ISC_DATE );
time_t decode_sql_unixtime( ISC_TIME );
time_t decode_unixtimestamp( ISC_TIMESTAMP );

//-----------------------------------------------------------------------------

}
