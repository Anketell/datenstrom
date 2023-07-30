//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/timestamp.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_support_gmtime )
{
   time_t    t = 24 * 60 * 60 * 32;
   struct tm tm;

   ds::time::gmtime( &t, &tm );

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

   time_t t = ds::time::timegm( &tm );

   EXPECT_EQ( t, 24 * 60 * 60 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_parse_iso_date )
{
   struct tm tm = { 0 };

   EXPECT_TRUE( ds::time::parse_iso_8601_date( "1970-01-01", &tm ) );

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

   EXPECT_TRUE( ds::time::parse_iso_8601_time( "01:01:01", &tm ) );

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

   EXPECT_TRUE( ds::time::parse_iso_8601( "1970-02-01 01:01:01", &tm ) );

   EXPECT_EQ( tm.tm_sec , 1 );
   EXPECT_EQ( tm.tm_min,  1 );
   EXPECT_EQ( tm.tm_hour, 1 );

   EXPECT_EQ( tm.tm_mday,  1 );
   EXPECT_EQ( tm.tm_mon,   1 );
   EXPECT_EQ( tm.tm_year, 70 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_fail_parse_iso_bad_date )
{
   struct tm tm = { 0 };

   EXPECT_FALSE( ds::time::parse_iso_8601_date( "bad date", &tm ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_fail_parse_iso_bad_time )
{
   struct tm tm = { 0 };

   EXPECT_FALSE( ds::time::parse_iso_8601_time( "bad time", &tm ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_fail_parse_iso_bad_date_time )
{
   struct tm tm = { 0 };

   EXPECT_FALSE( ds::time::parse_iso_8601( "bad date time", &tm ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_format_iso_datetime )
{
   struct tm tm =
   {
      1,
      1,
      1,
      1,
      1,
      70
   };

   char timestamp[ ds::time::datetime_len + 1 ];
   ds::time::format_iso_8601( &tm, timestamp );

   EXPECT_STREQ( timestamp, "1970-02-01 01:01:01" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_transform_iso_hires_time )
{
   constexpr char expected[] = "01:01:01.123456789";

   ds::time::stamp ts;

   EXPECT_TRUE( ds::time::parse_iso_8601_time( expected, &ts ) );

   EXPECT_EQ( ts.tm_sec, 1 );
   EXPECT_EQ( ts.tm_min, 1 );
   EXPECT_EQ( ts.tm_hour, 1 );

   EXPECT_EQ( ts.tm_mday, 0 );
   EXPECT_EQ( ts.tm_mon, 0 );
   EXPECT_EQ( ts.tm_year, 0 );

   EXPECT_EQ( ts.nano_sec, 123456789 );

   char timestamp[ ds::time::hires_datetime_len + 1 ];

   ds::time::format_iso_8601_time( &ts, timestamp );

   EXPECT_STREQ( timestamp, expected );

}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, time, should_transform_iso_hires_date_time )
{
   constexpr char expected[] = "1970-02-01 01:01:01.123456789";

   ds::time::stamp ts;

   EXPECT_TRUE( ds::time::parse_iso_8601( expected, &ts ) );

   EXPECT_EQ( ts.tm_sec, 1 );
   EXPECT_EQ( ts.tm_min, 1 );
   EXPECT_EQ( ts.tm_hour, 1 );

   EXPECT_EQ( ts.tm_mday, 1 );
   EXPECT_EQ( ts.tm_mon, 1 );
   EXPECT_EQ( ts.tm_year, 70 );

   EXPECT_EQ( ts.nano_sec, 123456789 );

   char timestamp[ ds::time::hires_datetime_len + 1 ];

   ds::time::format_iso_8601( &ts, timestamp );

   EXPECT_STREQ( timestamp, expected );

}

//-----------------------------------------------------------------------------
