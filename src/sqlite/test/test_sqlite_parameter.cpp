//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <sqlite/connection.h>
#include <sqlite/test/sqlite_test_data.h>
#include <test_model/object_serialise.h>
#include <string>

//-----------------------------------------------------------------------------

const ds::db::name_list_t named_parameters =
{
   "@string",
   "@double",
   "@float",
   "@u64",
   "@u32",
   "@u16",
   "@u8",
   "@i64",
   "@i32",
   "@i16",
   "@i8"
};

//-----------------------------------------------------------------------------

TEST( sqlite_parameter, should_insert_named )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( named, named_parameters );

      EXPECT_NO_THROW( insert_test << "hello2"
                                   << 10
                                   <<  9
                                   <<  8
                                   <<  7
                                   <<  6
                                   <<  5
                                   <<  4
                                   <<  3
                                   <<  2
                                   <<  1 );

      EXPECT_NO_THROW( insert_test.execute() );
   }

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_parameter, should_retrieve_named )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( named, named_parameters );

      EXPECT_NO_THROW( insert_test << "hello2"
                                   << 10
                                   <<  9
                                   <<  8
                                   <<  7
                                   <<  6
                                   <<  5
                                   <<  4
                                   <<  3
                                   <<  2
                                   <<  1 );

      EXPECT_NO_THROW( insert_test.execute() );
   }

   Object o;

   {
      ds::db::statement results_test = test_db( results );

      auto row = results_test.result();

      EXPECT_NO_THROW( row >> o );
   }

   EXPECT_EQ( o.m_i8, 1 );
   EXPECT_EQ( o.m_i16, 2 );
   EXPECT_EQ( o.m_i32, 3 );
   EXPECT_EQ( o.m_i64, 4 );
   EXPECT_EQ( o.m_u8, 5 );
   EXPECT_EQ( o.m_u16, 6 );
   EXPECT_EQ( o.m_u32, 7 );
   EXPECT_EQ( o.m_u64, 8 );
   EXPECT_EQ( o.m_f, 9 );
   EXPECT_EQ( o.m_d, 10 );
   EXPECT_EQ( o.m_hello, "hello2" );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------