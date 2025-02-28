//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/context.h>
#include <sqlite/test/sqlite_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, db, should_return_type )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_STREQ( test_db.type(), "sqlite" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, db, should_create_good_path )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, db, should_execute_batch )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.batch" ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, db, should_fail_create_bad_path )
{
   EXPECT_THROW( ds::db::connection test_db( "sqlite:///blah#test_db" ), std::runtime_error );
   EXPECT_THROW( ds::db::connection test_db( "sqlite://" ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_execute_simple )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db( "test.create" ).execute() );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_fail_execute_bad_sql )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_THROW( test_db( bad_sql ).execute(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_execute_query_parameters )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data[ 0 ] << ds::endr );

      EXPECT_NO_THROW( insert_test << 1 << 1 << 1 << 1 << 1
                                   << 1 << 1 << 1 << 1 << 1
                                   << "hello2" << "2020-05-14"
                                   << "14:05:20" << "2020-05-14 14:05:20"
                                   << ds::endr );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_return_execute_value )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert_id" );

      EXPECT_NO_THROW( insert_test << data[ 0 ] );
      EXPECT_EQ( static_cast< int >( insert_test.result() ), 1 );

      EXPECT_NO_THROW( insert_test << data[ 1 ] );
      EXPECT_EQ( static_cast< int >( insert_test.result() ), 2 );

      ds::db::statement value_test = test_db( "test.num_rows" );

      EXPECT_EQ( static_cast< int >( value_test.result() ), 2 );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_fail_query_too_many_parameters )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      insert_test << data[ 0 ];

      EXPECT_THROW( insert_test << ( int8_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int16_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int32_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int64_t )1, std::runtime_error );

      EXPECT_THROW( insert_test << ( uint8_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint16_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint32_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint64_t )1, std::runtime_error );

      EXPECT_THROW( insert_test << 8.5, std::runtime_error );

      EXPECT_THROW( insert_test << data[ 0 ].m_hello, std::runtime_error );
      EXPECT_THROW( insert_test << "hello", std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_fail_query_not_enough_parameters )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_THROW( insert_test.execute(), std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_provide_query_result_list )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

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

      ds::db::rowset result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data )
         result >> ds::endr;
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, statement, should_provide_query_result_row )
{
   static const char * expected_error = "SQLite result get column: No row available";

   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

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
      ds::db::statement test_stmt = test_db( "test.result" );

      uint64_t u64;

      EXPECT_NO_THROW( u64 = test_stmt( "Hello1" ) );
      EXPECT_EQ( u64, 64 );

      EXPECT_NO_THROW( u64 = test_stmt( "Hello2" ) );
      EXPECT_EQ( u64, 128 );

      EXPECT_THROW_ASSESS( u64 = test_stmt( "Hello3" ),
                           std::runtime_error,
                           EXPECT_STREQ( e.what(), expected_error ) );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, rowset, should_provide_query_data )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o <<ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         row >> o_db >> ds::endr;

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, rowset, should_support_unixtime )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data_alt )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data_alt )
      {
         Object_alt o_db = {};

         row >> o_db >> ds::endr;

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, rowset, should_provide_rows_affected )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

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

NAMESPACE_TEST( sqlite, rowset, should_return_query_data_not_available )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( "test.create" );

      EXPECT_EQ( ds::db::rowset(), create_test.result() );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, rowset, should_fail_query_wrong_column_count )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

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
         Object o_db = {};

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

NAMESPACE_TEST( sqlite, rowset, should_fail_query_wrong_column_type )
{
   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context test_db( test_con_str );

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

      std::string hello;

      EXPECT_THROW( row >> hello, std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
