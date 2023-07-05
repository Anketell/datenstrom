//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <ctime>
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
constexpr int time_len           = 8;
constexpr int hires_time_len     = time_len + hires_len;
constexpr int date_len           = 10;
constexpr int datetime_len       = 19;
constexpr int hires_datetime_len = datetime_len + hires_len;

//-----------------------------------------------------------------------------

}
