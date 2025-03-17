//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/context.h>
#include <test_model/object.h>
#include <db/test/test_mod_parameter.h>
#include <db/test/test_mod_config.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

const ds::db::name_list_t named_parameters =
{
   "datetime",
   "time",
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

TEST_P( Parameter, should_insert_named )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.named", named_parameters );

      EXPECT_NO_THROW( insert_test << "2020-05-14 14:05:20"
                                   << "14:05:20"
                                   << "2020-05-14"
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
                                   <<  1
                                   << ds::endr );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( Parameter, should_retrieve_named )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.named", named_parameters );

      EXPECT_NO_THROW( insert_test << "2020-05-14 14:05:20"
                                   << "14:05:20"
                                   << "2020-05-14"
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
                                   <<  1
                                   << ds::endr );
   }

   Object o;

   {
      ds::db::statement results_test = test_db( "test.results" );

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
   EXPECT_EQ( o.m_time, "14:05:20" );
   EXPECT_EQ( o.m_datetime, "2020-05-14 14:05:20" );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------