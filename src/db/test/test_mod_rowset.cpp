//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <test_utils/gtest.h>
#include <db/context.h>
#include <test_model/object.h>
#include <test_model/object_serialise.h>
#include <db/test/test_mod_rowset.h>
#include <db/test/test_mod_config.h>

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_provide_query_data )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      test::Object_list_t list = row;

      for ( int i = 0; i < list.size(); i++ )
         EXPECT_EQ( data[ i ], list[ i ] );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_throw_null_value_when_invalid )
{
   ds::db::context test_db(config->constr);

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch("test.create" ) );
   EXPECT_NO_THROW( test_db.execute_batch("test.insert_null" ) );

   {
      ds::db::rowset result = test_db( "test.results" ).result();

      test::Object o;

      EXPECT_THROW( o.m_i8       = result, ds::null_value );
      EXPECT_THROW( o.m_i16      = result, ds::null_value );
      EXPECT_THROW( o.m_i32      = result, ds::null_value );
      EXPECT_THROW( o.m_i64      = result, ds::null_value );
      EXPECT_THROW( o.m_u8       = result, ds::null_value );
      EXPECT_THROW( o.m_u16      = result, ds::null_value );
      EXPECT_THROW( o.m_u32      = result, ds::null_value );
      EXPECT_THROW( o.m_u64      = result, ds::null_value );
      EXPECT_THROW( o.m_f        = result, ds::null_value );
      EXPECT_THROW( o.m_d        = result, ds::null_value );
      EXPECT_THROW( o.m_hello    = result, ds::null_value );
      EXPECT_THROW( o.m_date     = result, ds::null_value );
      EXPECT_THROW( o.m_time     = result, ds::null_value );
      EXPECT_THROW( o.m_datetime = result, ds::null_value );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_get_null_when_invalid )
{
   ds::db::context test_db(config->constr);

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch("test.create" ) );
   EXPECT_NO_THROW( test_db.execute_batch("test.insert_null" ) );

   {
      ds::db::rowset result = test_db( "test.results" ).result();

      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
      EXPECT_TRUE( result.get_null() );
   }
   
   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_not_get_null_when_valid )
{
   ds::db::context test_db(config->constr);

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch("test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::rowset result = test_db( "test.results" ).result();

      test::Object o;

      EXPECT_FALSE( result.get_null() );
      result >> o.m_i8;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_i16;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_i32;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_i64;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u8;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u16;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u32;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u64;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_f;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_d;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_hello;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_date;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_time;
      EXPECT_FALSE( result.get_null() );
      result >> o.m_datetime;
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_get_null_not_null )
{
   ds::db::context test_db(config->constr);

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch("test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      auto o = data[ 0 ];

      insert_test << o.m_i8;
      insert_test.put_null();
      insert_test << o.m_i32;
      insert_test.put_null();
      insert_test << o.m_u8;
      insert_test.put_null();
      insert_test << o.m_u32;
      insert_test.put_null();
      insert_test << o.m_f;
      insert_test.put_null();
      insert_test << o.m_hello;
      insert_test.put_null();
      insert_test << o.m_time;
      insert_test.put_null();
      insert_test << ds::endr;

      insert_test << data[ 1 ] << ds::endr;

   ds::db::rowset result = test_db( "test.results" ).result();

      EXPECT_FALSE( result.get_null() );
      result >> o.m_i8;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_i32;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u8;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_u32;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_f;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_hello;
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      result >> o.m_time;
      EXPECT_TRUE( result.get_null() );
      result >> ds::endr;

      EXPECT_NO_THROW( result >> o >> ds::endr );

      EXPECT_EQ( o, data[ 1 ] );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_support_sub_second_time )
{
   ds::db::context test_db(config->constr);

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch("test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data_hires )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset result;

      EXPECT_NO_THROW( result = results_test.result() );

      test::Object_list_t list = result;

      for ( int i = 0; i < list.size(); i++ )
         EXPECT_EQ( data_hires[ i ], list[ i ] );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_provide_rows_affected )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_EQ( insert_test.result().rows_affected(), 1 );
      }
   }

   {
      ds::db::statement delete_test = test_db( "test.del_rows" );

      EXPECT_EQ( delete_test.result().rows_affected(), 2 );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_return_query_data_not_available )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement no_result_test = test_db( "test.no_result" );

      EXPECT_EQ( ds::db::rowset(), no_result_test.result() );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( RowSet, should_fail_query_wrong_column_count )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         test::Object o_db = {};

         if ( !row.eof() )
         {
            EXPECT_NO_THROW( row >> o_db );
            EXPECT_THROW( row >> o_db, std::runtime_error );
         }

         EXPECT_EQ( o, o_db );

         row >> ds::endr;
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
