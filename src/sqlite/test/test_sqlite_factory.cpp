//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/factory.h>
#include <sqlite/connection.h>

//-----------------------------------------------------------------------------

TEST( sqlite_factory, should_register_and_create )
{
   ds::db::factory factory;

   factory.register_db< ds::sqlite::connection >();

   ds::db::impl * db;

   EXPECT_NO_THROW( db = factory( "sqlite:" ) );
   EXPECT_NE( dynamic_cast< ds::sqlite::connection * >( db ), nullptr );
   EXPECT_NO_THROW( delete db );
}

//-----------------------------------------------------------------------------
