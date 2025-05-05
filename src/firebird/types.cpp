//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/types.h>
#include <dsutil/timestamp.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

stmt_t::~stmt_t( void )
{
   if ( stmt && ( xsqlda == nullptr || xsqlda->sqld != 0 ) )
   {
      ISC_STATUS status[ status_vector_length ];

      isc_dsql_free_statement( status, &stmt, DSQL_drop );

      check_status( "Firebird drop statement", status );
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

//-----------------------------------------------------------------------------

ISC_DATE encode_sql_date( const char * date )
{
   struct tm tm = { 0 };

   ds::time::parse_iso_8601_date( date, &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

ISC_TIME encode_sql_time( const char * time )
{
   ds::time::stamp ts;

   ds::time::parse_iso_8601_time( time, &ts );

   ISC_TIME isc_time;

   isc_encode_sql_time( &ts, &isc_time );

   isc_time += ts.nano_sec / 100000;

   return isc_time;
}

//-----------------------------------------------------------------------------

ISC_TIMESTAMP encode_timestamp( const char * time )
{
   ds::time::stamp ts;

   ds::time::parse_iso_8601( time, &ts );

   ISC_TIMESTAMP isc_timestamp;

   isc_encode_timestamp( &ts, &isc_timestamp );

   isc_timestamp.timestamp_time += ts.nano_sec / 100000;

   return isc_timestamp;
}

//-----------------------------------------------------------------------------

ISC_DATE encode_sql_unixdate( time_t t )
{
   struct tm tm = { 0 };

   ds::time::gmtime( &t, &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

ISC_TIME encode_sql_unixtime( time_t t )
{
   struct tm tm = { 0 };

   ds::time::gmtime( &t, &tm );

   ISC_TIME isc_time;

   isc_encode_sql_time( &tm, &isc_time );

   return isc_time;
}

//-----------------------------------------------------------------------------

ISC_TIMESTAMP encode_unixtimestamp( time_t t )
{
   struct tm tm = { 0 };

   ds::time::gmtime( &t, &tm );

   ISC_TIMESTAMP isc_timestamp;

   isc_encode_timestamp( &tm, &isc_timestamp );

   return isc_timestamp;
}

//-----------------------------------------------------------------------------

std::string decode_sql_date( ISC_DATE isc_date )
{
   struct tm tm = { 0 };

   isc_decode_sql_date( &isc_date, &tm );

   char date[ 11 ];

   ds::time::format_iso_8601_date( &tm, date );

   return date;
}

//-----------------------------------------------------------------------------

static std::string sub_seconds( ISC_TIME isc_time )
{
   uint32_t magnitude = 10000;
   uint32_t sub_sec   = isc_time % magnitude;
   if ( !sub_sec )
      return "";

   std::string res = ".";

   do
   {
      magnitude /= 10;
      res       += sub_sec / magnitude + '0';
      sub_sec   %= magnitude;
   }
   while ( sub_sec );

   return res;
}

//-----------------------------------------------------------------------------

std::string decode_sql_time( ISC_TIME isc_time )
{
   struct tm tm = { 0 };

   isc_decode_sql_time( &isc_time, &tm );

   char time[ 9 ];

   ds::time::format_iso_8601_time( &tm, time );

   return time + sub_seconds( isc_time );
}

//-----------------------------------------------------------------------------

std::string decode_timestamp( ISC_TIMESTAMP isc_timestamp )
{
   struct tm tm = { 0 };

   isc_decode_timestamp( &isc_timestamp, &tm );

   char timestamp[ 20 ];

   ds::time::format_iso_8601( &tm, timestamp );

   return timestamp + sub_seconds( isc_timestamp.timestamp_time );
}

//-----------------------------------------------------------------------------

time_t decode_sql_unixdate( ISC_DATE isc_date )
{
   struct tm tm = { 0 };

   isc_decode_sql_date( &isc_date, &tm );

   return ds::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

time_t decode_sql_unixtime( ISC_TIME isc_time )
{
   struct tm tm = { 0 };

   isc_decode_sql_time( &isc_time, &tm );

   tm.tm_year = 70;
   tm.tm_mon  = 0;
   tm.tm_mday = 1;

   return ds::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

time_t decode_unixtimestamp( ISC_TIMESTAMP isc_timestamp )
{
   struct tm tm = { 0 };

   isc_decode_timestamp( &isc_timestamp, &tm );

   return ds::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

}
