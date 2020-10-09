//-----------------------------------------------------------------------------

#include <firebird/types.h>
#include <util/time.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

ISC_DATE encode_sql_date( const char * date )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601_date( date, &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

ISC_TIME encode_sql_time( const char * time )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601_time( time, &tm );

   ISC_TIME isc_time;

   isc_encode_sql_time( &tm, &isc_time );

   return isc_time;
}

//-----------------------------------------------------------------------------

ISC_TIMESTAMP encode_timestamp( const char * time )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601( time, &tm );

   ISC_TIMESTAMP isc_timestamp;

   isc_encode_timestamp( &tm, &isc_timestamp );

   return isc_timestamp;
}

//-----------------------------------------------------------------------------

ISC_DATE encode_sql_unixdate( time_t t )
{
   struct tm tm = { 0 };

   util::time::gmtime( &t, &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

ISC_TIME encode_sql_unixtime( time_t t )
{
   struct tm tm = { 0 };

   util::time::gmtime( &t, &tm );

   ISC_TIME isc_time;

   isc_encode_sql_time( &tm, &isc_time );

   return isc_time;
}

//-----------------------------------------------------------------------------

ISC_TIMESTAMP encode_sql_unixtimestamp( time_t t )
{
   struct tm tm = { 0 };

   util::time::gmtime( &t, &tm );

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

   util::time::format_iso_8601_date( &tm, date );

   return date;
}

//-----------------------------------------------------------------------------

std::string decode_sql_time( ISC_TIME isc_time )
{
   struct tm tm = { 0 };

   isc_decode_sql_time( &isc_time, &tm );

   char time[ 9 ];

   util::time::format_iso_8601_time( &tm, time );

   return time;
}

//-----------------------------------------------------------------------------

std::string decode_timestamp( ISC_TIMESTAMP isc_timestamp )
{
   struct tm tm = { 0 };

   isc_decode_timestamp( &isc_timestamp, &tm );

   char timestamp[ 20 ];

   util::time::format_iso_8601( &tm, timestamp );

   return timestamp;
}

//-----------------------------------------------------------------------------

time_t decode_sql_unixdate( ISC_DATE isc_date )
{
   struct tm tm = { 0 };

   isc_decode_sql_date( &isc_date, &tm );

   return util::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

time_t decode_sql_unixtime( ISC_TIME isc_time )
{
   struct tm tm = { 0 };

   isc_decode_sql_time( &isc_time, &tm );

   tm.tm_year = 70;
   tm.tm_mon  = 0;
   tm.tm_mday = 1;

   return util::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

time_t decode_unixtimestamp( ISC_TIMESTAMP isc_timestamp )
{
   struct tm tm = { 0 };

   isc_decode_timestamp( &isc_timestamp, &tm );

   return util::time::timegm( &tm );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
