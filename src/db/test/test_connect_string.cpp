//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/connect_string.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_connect_string )
{
   static const char connect_string[] = "type://server:1234//path?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "/path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_connect_string_no_path )
{
   static const char connect_string[] = "type://server:1234?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(), "type" );
   EXPECT_STREQ( params[ "server" ].c_str(), "server" );
   EXPECT_STREQ( params[ "port" ].c_str(), "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(), "" );
   EXPECT_STREQ( params[ "name" ].c_str(), "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_support_full_windows_path_with_port )
{
   static const char connect_string[] = "type://server:1234/C:\\path?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "C:\\path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_support_full_windows_path_without_port )
{
   static const char connect_string[] = "type://server/C:\\path?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "C:\\path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_no_server_connect_string )
{
   static const char connect_string[] = "type:////path?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "/path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_parse_no_server_full_windows_path_connect_string )
{
   static const char connect_string[] = "type:///C:\\path\\?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(), "type" );
   EXPECT_STREQ( params[ "server" ].c_str(), "" );
   EXPECT_STREQ( params[ "port" ].c_str(), "" );
   EXPECT_STREQ( params[ "path" ].c_str(), "C:\\path\\" );
   EXPECT_STREQ( params[ "name" ].c_str(), "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_fail_poorly_formed )
{
   static const char connect_string[] = "type://ser&ver:1234?name=value#database";
   ds::db::connect_params_t params;

   EXPECT_THROW( params = ds::db::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, connect_string, should_fail_null_param )
{
   static const char connect_string[] = "type://server:1234?name=value=asd#database";
   ds::db::connect_params_t params;

   EXPECT_THROW( params = ds::db::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------


