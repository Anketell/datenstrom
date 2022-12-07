//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <ctime>

//-----------------------------------------------------------------------------

namespace ds::time
{

//-----------------------------------------------------------------------------

constexpr int time_len     = 8;
constexpr int date_len     = 10;
constexpr int datetime_len = 19;

//-----------------------------------------------------------------------------

void   gmtime( const time_t * t,  struct tm * tm );
time_t timegm( const struct tm * tm );

bool parse_iso_8601_date( const char * s, struct tm * tm );
bool parse_iso_8601_time( const char * s, struct tm * tm );
bool parse_iso_8601( const char * s, struct tm * tm );
void format_iso_8601_date( const struct tm * tm, char * s );
void format_iso_8601_time( const struct tm * tm, char * s );
void format_iso_8601( const struct tm * tm, char * s );

//-----------------------------------------------------------------------------

}
