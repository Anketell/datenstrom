//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <dsutil/time.h>
#include <cstdint>

//-----------------------------------------------------------------------------

namespace ds::time
{

//-----------------------------------------------------------------------------

struct stamp : tm
{
   uint32_t nano_sec;
};

//-----------------------------------------------------------------------------

constexpr int hires_len          = 10;
constexpr int hires_time_len     = time_len + hires_len;
constexpr int hires_datetime_len = datetime_len + hires_len;

//-----------------------------------------------------------------------------

bool parse_iso_8601_date( const char * s, stamp * ts );
bool parse_iso_8601_time( const char * s, stamp * ts );
bool parse_iso_8601( const char * s, stamp * ts );
void format_iso_8601_date( const stamp * ts, char * s );
void format_iso_8601_time( const stamp * ts, char * s );
void format_iso_8601( const stamp * ts, char * s );

//-----------------------------------------------------------------------------

}
