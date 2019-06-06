//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/factory.h>
#include <sqlite/database.h>

//-----------------------------------------------------------------------------

TEST( sqlite_factory, should_register_and_create )
{
   ds::db::factory factory;

   factory.register_db< ds::sqlite::database >();

   ds::db::database * db;

   EXPECT_NO_THROW( db = factory( "sqlite:" ) );
   EXPECT_NE( dynamic_cast< ds::db::database * >( db ), nullptr );
   EXPECT_NO_THROW( delete db );
}

//-----------------------------------------------------------------------------
