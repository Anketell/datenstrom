//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/time.h>
#include <stdio.h>
#include <cctype>

//-----------------------------------------------------------------------------

namespace ds::time
{

//-----------------------------------------------------------------------------

void gmtime( const time_t * t,  struct tm * tm )
{
    gmtime_s( tm, t );
}

//-----------------------------------------------------------------------------

time_t timegm( const struct tm * tm )
{
    return _mkgmtime( const_cast< struct tm * >( tm ) );
}

//-----------------------------------------------------------------------------

bool parse_iso_8601_date( const char * s, struct tm * tm )
{
    int year;
    int month;
    int day;

    if ( sscanf_s( s, "%d-%d-%d", &year, &month, &day ) != 3 )
        return false;

    tm->tm_year = year - 1900;
    tm->tm_mon  = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = 0;
    tm->tm_min  = 0;
    tm->tm_sec  = 0;

    return true;
}

//-----------------------------------------------------------------------------

bool parse_iso_8601_time( const char * s, struct tm * tm )
{
    int hour;
    int minute;
    int second;

    if ( sscanf_s( s, "%d:%d:%dZ", &hour, &minute, &second ) != 3 )
        return false;

    tm->tm_year = 0;
    tm->tm_mon  = 0;
    tm->tm_mday = 0;
    tm->tm_hour = hour;
    tm->tm_min  = minute;
    tm->tm_sec  = second;

    return true;
}

//-----------------------------------------------------------------------------

bool parse_iso_8601( const char * s, struct tm * tm )
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    if ( sscanf_s( s, "%d-%d-%d %d:%d:%dZ", &year, &month, &day, &hour, &minute, &second ) != 6 )
        return false;

    tm->tm_year = year - 1900;
    tm->tm_mon  = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = hour;
    tm->tm_min  = minute;
    tm->tm_sec  = second;

    return true;
}

//-----------------------------------------------------------------------------

void format_iso_8601_date( const struct tm * tm, char * s )
{
   strftime( s, 11, "%F", tm );
}

//-----------------------------------------------------------------------------

void format_iso_8601_time( const struct tm * tm, char * s )
{
   strftime( s, 9, "%T", tm );
}

//-----------------------------------------------------------------------------

void format_iso_8601( const struct tm * tm, char * s )
{
   strftime( s, 20, "%F %T", tm );
}

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

void set_nano_sec( char * s, uint32_t nano_sec )
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

}
