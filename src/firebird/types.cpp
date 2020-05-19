//-----------------------------------------------------------------------------

#include <firebird/types.h>

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

   strptime( date, "%F", &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

ISC_DATE encode_sql_unixdate( time_t t )
{
   struct tm tm = { 0 };

   gmtime_r( &t, &tm );

   ISC_DATE isc_date;

   isc_encode_sql_date( &tm, &isc_date );

   return isc_date;
}

//-----------------------------------------------------------------------------

std::string decode_sql_date( ISC_DATE isc_date )
{
   struct tm tm = { 0 };

   isc_decode_sql_date( &isc_date, &tm );

   char date[ 11 ];

   strftime( date, sizeof( date ), "%F", &tm );

   return date;
}

//-----------------------------------------------------------------------------

time_t decode_sql_unixdate( ISC_DATE isc_date )
{
   struct tm tm = { 0 };

   isc_decode_sql_date( &isc_date, &tm );

   return timegm( &tm );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
