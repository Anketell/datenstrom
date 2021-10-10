//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <util/time.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_support_gmtime )
{
   time_t    t = 24 * 60 * 60 * 32; 
   struct tm tm;

   util::time::gmtime( &t, &tm );

   EXPECT_EQ( tm.tm_sec,  0 );
   EXPECT_EQ( tm.tm_min,  0 );
   EXPECT_EQ( tm.tm_hour, 0 );

   EXPECT_EQ( tm.tm_mday,  2 );
   EXPECT_EQ( tm.tm_mon,   1 );
   EXPECT_EQ( tm.tm_year, 70 );

   EXPECT_EQ( tm.tm_wday,  1 );
   EXPECT_EQ( tm.tm_yday,  32 );
   EXPECT_EQ( tm.tm_isdst, 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_support_timegm )
{
   struct tm tm =
   {
      0,
      0,
      0,
      2,
      0,
      70,
      4,
      0,
      0
   };

   time_t t = util::time::timegm( &tm );

   EXPECT_EQ( t, 24 * 60 * 60 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_parse_iso_date )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601_date( "1970-01-01", &tm );

   EXPECT_EQ( tm.tm_sec , 0 );
   EXPECT_EQ( tm.tm_min,  0 );
   EXPECT_EQ( tm.tm_hour, 0 );

   EXPECT_EQ( tm.tm_mday,  1 );
   EXPECT_EQ( tm.tm_mon,   0 );
   EXPECT_EQ( tm.tm_year, 70 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_parse_iso_time )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601_time( "01:01:01", &tm );

   EXPECT_EQ( tm.tm_sec,  1 );
   EXPECT_EQ( tm.tm_min,  1 );
   EXPECT_EQ( tm.tm_hour, 1 );

   EXPECT_EQ( tm.tm_mday, 0 );
   EXPECT_EQ( tm.tm_mon,  0 );
   EXPECT_EQ( tm.tm_year, 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_parse_iso_datetime )
{
   struct tm tm = { 0 };

   util::time::parse_iso_8601( "1970-02-01 01:01:01", &tm );

   EXPECT_EQ( tm.tm_sec , 1 );
   EXPECT_EQ( tm.tm_min,  1 );
   EXPECT_EQ( tm.tm_hour, 1 );

   EXPECT_EQ( tm.tm_mday,  1 );
   EXPECT_EQ( tm.tm_mon,   1 );
   EXPECT_EQ( tm.tm_year, 70 );
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
