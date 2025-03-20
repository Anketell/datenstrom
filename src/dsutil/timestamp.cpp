//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/timestamp.h>
#include <stdexcept>
#include <cctype>

//-----------------------------------------------------------------------------

namespace ds::time
{

//-----------------------------------------------------------------------------

bool parse_iso_8601_date( const char * s, stamp * ts )
{
   ts->nano_sec = 0;
   return parse_iso_8601_date( s, static_cast< struct tm * >( ts ) );
}

//-----------------------------------------------------------------------------

static uint32_t pow_u32( uint32_t base, uint32_t exp )
{
   uint32_t res  = 1;
   uint32_t comp = base;

   while ( exp )
   {
      if ( exp & 1 )
         res  *= comp;

      comp *= comp;
      exp >>= 1;
   }

   return res;
}

//-----------------------------------------------------------------------------

static uint32_t get_nano_sec( const char * s )
{
   uint32_t nano_sec = 0;

   if ( *s++ != '.' )
      return nano_sec;

   int index;

   for ( index = 1; index < hires_len; index++ )
   {
      if ( !*s || !isdigit( *s ) )
         break;

      nano_sec = nano_sec * 10 + *s++ - '0';
   }

   return nano_sec * pow_u32( 10, hires_len - index );
}

//-----------------------------------------------------------------------------

bool parse_iso_8601_time( const char * s, stamp * ts )
{
   if ( !parse_iso_8601_time( s, static_cast< struct tm * >( ts ) ) )
      return false;

   ts->nano_sec = get_nano_sec( s + time_len );

   return true;
}

//-----------------------------------------------------------------------------

bool parse_iso_8601( const char * s, stamp * ts )
{
   if ( !parse_iso_8601( s, static_cast< struct tm * >( ts ) ) )
      return false;

   ts->nano_sec = get_nano_sec( s + datetime_len );

   return true;
}

//-----------------------------------------------------------------------------

void format_iso_8601_date( const stamp * ts, char * s )
{
   format_iso_8601_date( static_cast< const struct tm * >( ts ), s );
}

//-----------------------------------------------------------------------------

static void set_nano_sec( char * s, uint32_t nano_sec )
{
   if ( !nano_sec )
      return;

   *s++ = '.';

   uint32_t power = 100000000;

   do
   {
      *s++      = nano_sec / power + '0';
      nano_sec %= power;
      power    /= 10;
   } while ( nano_sec );

   *s = '\0';
}

//-----------------------------------------------------------------------------

void format_iso_8601_time( const stamp * ts, char * s )
{
   format_iso_8601_time( static_cast< const struct tm * >( ts ), s );
   set_nano_sec( s + time_len, ts->nano_sec );
}

//-----------------------------------------------------------------------------

void format_iso_8601( const stamp * ts, char * s )
{
   format_iso_8601( static_cast< const struct tm * >( ts ), s );
   set_nano_sec( s + datetime_len, ts->nano_sec );
}

//-----------------------------------------------------------------------------

static void set_nano_sec( char * s, uint32_t nano_sec, uint8_t decimals )
{
   if ( !decimals )
      return;

   *s++ = '.';

   uint32_t power = 100000000;

   while ( power && decimals > 1 )
   {
      *s++      = nano_sec / power + '0';
      nano_sec %= power;
      power    /= 10;
      decimals--;
   };

   if ( power && decimals == 1 )
   {
      *s++ = nano_sec / power + ( ( nano_sec % power ) + power / 2 ) / power + '0';
      decimals--;
   }

   while ( decimals-- )
   {
      *s++ = '0';
   }

   *s = '\0';
}

//-----------------------------------------------------------------------------

void format_iso_8601_time( const stamp * ts, char * s, uint8_t decimals )
{
   format_iso_8601_time( static_cast< const struct tm * >( ts ), s );
   set_nano_sec( s + time_len, ts->nano_sec, decimals );
}

//-----------------------------------------------------------------------------

void format_iso_8601( const stamp * ts, char * s, uint8_t decimals )
{
   format_iso_8601( static_cast< const struct tm * >( ts ), s );
   set_nano_sec( s + datetime_len, ts->nano_sec, decimals );
}

//-----------------------------------------------------------------------------

bool reformat_iso_8601_time( std::string & s )
{
   stamp ts;

   if ( !parse_iso_8601_time( s.c_str(), &ts ) )
      return false;

   char res[ hires_time_len + 1 ];

   format_iso_8601_time( &ts, res );

   s = res;

   return true;
}

//-----------------------------------------------------------------------------

bool reformat_iso_8601_time( std::string & s, uint8_t decimals )
{
   stamp ts;
   
   if ( !parse_iso_8601_time( s.c_str(), &ts ) )
      return false;

   char res[ hires_time_len + 257 ];

   format_iso_8601_time( &ts, res, decimals );

   s = res;

   return true;
};

//-----------------------------------------------------------------------------

bool reformat_iso_8601( std::string & s )
{
   stamp ts;

   if ( !parse_iso_8601( s.c_str(), &ts ) )
      return false;

   char res[ hires_datetime_len + 1 ];

   format_iso_8601( &ts, res );

   s = res;

   return true;
}

//-----------------------------------------------------------------------------

bool reformat_iso_8601( std::string & s, uint8_t decimals )
{
   stamp ts;

   if ( !parse_iso_8601( s.c_str(), &ts ) )
      return false;

   char res[ hires_datetime_len + 257 ];

   format_iso_8601( &ts, res, decimals );

   s = res;

   return true;
}

//-----------------------------------------------------------------------------

}
