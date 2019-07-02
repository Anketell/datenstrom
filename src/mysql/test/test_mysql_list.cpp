//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <mysql/test/mysql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( mysql_db_list, should_provide_iterator )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_EQ( insert_test.result().rows_affected(), 1 );
      }
   }

   {
      int i = 0;
      for ( Object o : test_db( results ) )
         EXPECT_EQ( o, data[ i++ ] );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
