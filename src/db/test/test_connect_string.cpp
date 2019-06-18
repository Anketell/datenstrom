//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connect_string.h>

//-----------------------------------------------------------------------------

TEST( db_connect_string, should_parse_connect_string )
{
   static const char connect_string[] = "type://location:1234?name=value#database";
   ds::db::connect_params_t params = ds::db::parse_connect_string( connect_string );

   EXPECT_STREQ( params[ "type" ].c_str(),     "type" );
   EXPECT_STREQ( params[ "location" ].c_str(), "location" );
   EXPECT_STREQ( params[ "port" ].c_str(),     "1234" );
   EXPECT_STREQ( params[ "name" ].c_str(),     "value" );
   EXPECT_STREQ( params[ "database" ].c_str(), "database" );
}

//-----------------------------------------------------------------------------

TEST( db_connect_string, should_fail_poorly_formed )
{
   static const char connect_string[] = "type://loc&ation:1234?name=value#database";
   ds::db::connect_params_t params;

   EXPECT_THROW( params = ds::db::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

TEST( db_connect_string, should_fail_null_param )
{
   static const char connect_string[] = "type://location:1234?name=value=asd#database";
   ds::db::connect_params_t params;

   EXPECT_THROW( params = ds::db::parse_connect_string( connect_string ), std::invalid_argument );
}

//-----------------------------------------------------------------------------


