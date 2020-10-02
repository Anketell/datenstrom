//-----------------------------------------------------------------------------

#include <util/time.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

namespace util
{

//-----------------------------------------------------------------------------

namespace time
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

void parse_iso_8601( const char * s, struct tm * tm )
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    sscanf_s( s, "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second );

    tm->tm_year = year - 1900;
    tm->tm_mon  = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = hour;
    tm->tm_min  = minute;
    tm->tm_sec  = second;
}

//-----------------------------------------------------------------------------

void format_iso_8601( const struct tm * tm, char * s )
{
    strftime( s, 11, "%F", tm );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
