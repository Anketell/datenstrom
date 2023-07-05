//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/connect_string.h>
#include <dsutil/url_encode.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_connect_string )
{
   static const char connect_string[] = "type://server:1234//path?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "/path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_connect_string_no_path )
{
   static const char connect_string[] = "type://server:1234?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(), "type" );
   EXPECT_STREQ( params[ "server" ].c_str(), "server" );
   EXPECT_STREQ( params[ "port" ].c_str(), "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(), "" );
   EXPECT_STREQ( params[ "name" ].c_str(), "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_support_full_windows_path_with_port )
{
   static const char connect_string[] = "type://server:1234/C:\\path?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "1234" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "C:\\path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_support_full_windows_path_without_port )
{
   static const char connect_string[] = "type://server/C:\\path?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "server" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "C:\\path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_no_server_connect_string )
{
   static const char connect_string[] = "type:////path?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "/path" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_no_server_no_params_connect_string )
{
   static const char connect_string[] = "type:////path/";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "/path/" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "" );
   EXPECT_STREQ( params[ "database" ].c_str(), "" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_no_server_full_windows_path_connect_string )
{
   static const char connect_string[] = "type:///C:\\path\\?name=value#database";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(), "type" );
   EXPECT_STREQ( params[ "server" ].c_str(), "" );
   EXPECT_STREQ( params[ "port" ].c_str(), "" );
   EXPECT_STREQ( params[ "path" ].c_str(), "C:\\path\\" );
   EXPECT_STREQ( params[ "name" ].c_str(), "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_fail_poorly_formed )
{
   static const char connect_string[] = "type://ser&ver:1234?name=value#database";
   ds::connect_params_t params;

   EXPECT_THROW( params = ds::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_fail_null_param )
{
   static const char connect_string[] = "type://server:1234?name=value=asd#database";
   ds::connect_params_t params;

   EXPECT_THROW( params = ds::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_tls_connect_string )
{
   static const char connect_string[] = "TLS://127.0.0.1:40002?cert=My Cert";
   ds::connect_params_t params = ds::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(), "TLS" );
   EXPECT_STREQ( params[ "server" ].c_str(), "127.0.0.1" );
   EXPECT_STREQ( params[ "port" ].c_str(), "40002" );
   EXPECT_STREQ( params[ "cert" ].c_str(), "My Cert" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, connect_string, should_parse_escaped_connect_string )
{
   std::string connect_string;

   connect_string = "mssql://ITWCONS-0195/SQLEXPRESS#PM_Analytics";

   ds::connect_params_t params;

   EXPECT_NO_THROW( params = ds::parse_connect_string( connect_string ) );

   EXPECT_STREQ( params[ "type" ].c_str(),     "mssql" );
   EXPECT_STREQ( params[ "server" ].c_str(),   "ITWCONS-0195" );
   EXPECT_STREQ( params[ "path" ].c_str(),     "SQLEXPRESS" );
   EXPECT_STREQ( params[ "database" ].c_str(), "PM_Analytics" );
}

//-----------------------------------------------------------------------------

