//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <sqlite/test/sqlite_test_data.h>
#include <test_model/object_serialise.h>
#include <string>

//-----------------------------------------------------------------------------

const ds::db::name_list_t named_parameters =
{
   "date",
   "string",
   "double",
   "float",
   "u64",
   "u32",
   "u16",
   "u8",
   "i64",
   "i32",
   "i16",
   "i8"
};

//-----------------------------------------------------------------------------

TEST( sqlite_parameter, should_insert_named )
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
      ds::db::statement insert_test = test_db( named, named_parameters );

      EXPECT_NO_THROW( insert_test << "2020-05-14"
                                   << "hello2"
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

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_parameter, should_retrieve_named )
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
      ds::db::statement insert_test = test_db( named, named_parameters );

      EXPECT_NO_THROW( insert_test << "2020-05-14"
                                   << "hello2"
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
   EXPECT_EQ( o.m_date, "2020-05-14" );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------