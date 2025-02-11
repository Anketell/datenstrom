//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/connection.h>
#include <mysql/test/mysql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, db, should_return_type )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_STREQ( test_db.type(), "mysql" );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, db, should_create_good_path )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, db, should_execute_batch )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( batch ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, db, should_fail_create_bad_con_str )
{
   EXPECT_THROW( ds::db::connection test_db( "mysql://blah?username=root" ), std::runtime_error );
   EXPECT_THROW( ds::db::connection test_db( "mysql://" ), std::invalid_argument );
   EXPECT_THROW( ds::db::connection test_db( "mysql://blah" ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_execute_simple )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db( simple_create ).execute() );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_fail_execute_bad_sql )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_THROW( test_db( bad_sql ).execute(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_return_execute_value )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      EXPECT_NO_THROW( insert_test << data[ 0 ] );
      EXPECT_EQ( static_cast< int >( insert_test.result() ), 1 );

      EXPECT_NO_THROW( insert_test << data[ 1 ] );
      EXPECT_EQ( static_cast< int >( insert_test.result() ), 2 );

      ds::db::statement value_test = test_db( num_rows );

      EXPECT_EQ( static_cast< int >( value_test.result() ), 2 );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_execute_query_parameters )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      EXPECT_NO_THROW( insert_test << data[ 0 ] << ds::endr );

      EXPECT_NO_THROW( insert_test << 1 << 1 << 1 << 1 << 1
                                   << 1 << 1 << 1 << 1 << 1
                                   << "hello2"
                                   << "2020-05-14"
                                   << "14:05:20"
                                   << "2020-05-14 14:05:20"
                                   << ds::endr );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_fail_query_too_many_parameters )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

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

NAMESPACE_TEST( mysql, statement, should_fail_query_not_enough_parameters )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      EXPECT_THROW( insert_test.execute(), std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_provide_query_result_row )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::rowset result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data )
         result >> ds::endr;
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, statement, should_fail_bad_query )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );
   EXPECT_THROW( test_db.execute_batch( create ), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, result, should_provide_query_data )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::rowset result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         result >> o_db >> ds::endr;

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, result, should_support_unixtime )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert_alt );

      for ( auto o : data_alt )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( results_alt );

      ds::db::rowset result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data_alt )
      {
         Object_alt o_db = {};

         result >> o_db >> ds::endr;

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, result, should_provide_rows_affected )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_EQ( insert_test.result().rows_affected(), 1 );
      }
   }

   ds::db::statement delete_test = test_db( del_rows );

   EXPECT_EQ( delete_test.result().rows_affected(), 2 );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, result, should_return_query_data_not_available )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   ds::db::statement create_test = test_db( simple_create );

   EXPECT_EQ( ds::db::rowset(), create_test.result() );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, result, should_fail_query_wrong_column_count )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         EXPECT_NO_THROW( row >> o_db );
         EXPECT_THROW( row >> o_db, std::runtime_error );

         EXPECT_EQ( o, o_db );

         row >> ds::endr;
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
