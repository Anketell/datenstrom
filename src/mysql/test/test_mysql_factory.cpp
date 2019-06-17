//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/factory.h>
#include <mysql/constructor.h>

//-----------------------------------------------------------------------------

TEST( mysql_factory, should_register_and_create )
{
   ds::db::factory factory;

   factory.register_db< ds::mysql::connection >();

   ds::db::impl * db;

   EXPECT_NO_THROW( db = factory( "mysql://127.0.0.1:3306?username=root" ) );
   EXPECT_NE( dynamic_cast< ds::mysql::connection * >( db ), nullptr );
   EXPECT_NO_THROW( delete db );
}

//-----------------------------------------------------------------------------
