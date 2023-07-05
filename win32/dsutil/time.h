//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <dsutil/timestamp.h>

//-----------------------------------------------------------------------------

namespace ds::time
{

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

bool parse_iso_8601_date( const char * s, stamp * ts );
bool parse_iso_8601_time( const char * s, stamp * ts );
bool parse_iso_8601( const char * s, stamp * ts );
void format_iso_8601_date( const stamp * ts, char * s );
void format_iso_8601_time( const stamp * ts, char * s );
void format_iso_8601( const stamp * ts, char * s );

//-----------------------------------------------------------------------------

}
