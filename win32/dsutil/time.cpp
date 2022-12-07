//-----------------------------------------------------------------------------

#include <dsutil/time.h>
#include <stdio.h>

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

}
