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

      EXPECT_NO_THROW( insert_test << data[ 0 ].m_datetime
                                   << data[ 0 ].m_time
                                   << data[ 0 ].m_date
                                   << data[ 0 ].m_hello
                                   << data[ 0 ].m_d
                                   << data[ 0 ].m_f
                                   << data[ 0 ].m_u64
                                   << data[ 0 ].m_u32
                                   << data[ 0 ].m_u16
                                   << data[ 0 ].m_u8
                                   << data[ 0 ].m_i64
                                   << data[ 0 ].m_i32
                                   << data[ 0 ].m_i16
                                   << data[ 0 ].m_i8
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

      EXPECT_NO_THROW( insert_test << data[ 0 ].m_datetime
                                   << data[ 0 ].m_time
                                   << data[ 0 ].m_date
                                   << data[ 0 ].m_hello
                                   << data[ 0 ].m_d
                                   << data[ 0 ].m_f
                                   << data[ 0 ].m_u64
                                   << data[ 0 ].m_u32
                                   << data[ 0 ].m_u16
                                   << data[ 0 ].m_u8
                                   << data[ 0 ].m_i64
                                   << data[ 0 ].m_i32
                                   << data[ 0 ].m_i16
                                   << data[ 0 ].m_i8
                                   << ds::endr );
   }

   Object o;

   {
      ds::db::statement results_test = test_db( "test.results" );

      auto row = results_test.result();

      EXPECT_NO_THROW( row >> o );
   }

   EXPECT_EQ( o.m_i8,       data[ 0 ].m_i8 );
   EXPECT_EQ( o.m_i16,      data[ 0 ].m_i16 );
   EXPECT_EQ( o.m_i32,      data[ 0 ].m_i32 );
   EXPECT_EQ( o.m_i64,      data[ 0 ].m_i64 );
   EXPECT_EQ( o.m_u8,       data[ 0 ].m_u8 );
   EXPECT_EQ( o.m_u16,      data[ 0 ].m_u16 );
   EXPECT_EQ( o.m_u32,      data[ 0 ].m_u32 );
   EXPECT_EQ( o.m_u64,      data[ 0 ].m_u64 );
   EXPECT_EQ( o.m_f,        data[ 0 ].m_f );
   EXPECT_EQ( o.m_d,        data[ 0 ].m_d );
   EXPECT_EQ( o.m_hello,    data[ 0 ].m_hello );
   EXPECT_EQ( o.m_date,     data[ 0 ].m_date );
   EXPECT_EQ( o.m_time,     data[ 0 ].m_time );
   EXPECT_EQ( o.m_datetime, data[ 0 ].m_datetime );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------