//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <firebird/test/firebird_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

const ds::db::name_list_t named_parameters =
{
   "string",
   "double_",
   "float_",
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

TEST( firebird_parameter, should_insert_named )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( named, named_parameters );

      EXPECT_NO_THROW( insert_test << "hello2"
                                   << double( 10.0 )
                                   << float( 9 )
                                   << uint64_t( 8 )
                                   << uint32_t( 7 )
                                   << uint16_t( 6 )
                                   << uint8_t( 5 )
                                   << int64_t( 4 )
                                   << int32_t( 3 )
                                   << int16_t( 2 )
                                   << int8_t( 1 ) );

      EXPECT_NO_THROW( insert_test.execute() );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( firebird_parameter, should_retrieve_named )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

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

      EXPECT_EQ( insert_test.result().rows_affected(), 1 );
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

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------