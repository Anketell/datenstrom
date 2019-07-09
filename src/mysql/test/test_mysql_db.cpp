//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <mysql/test/mysql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( mysql_db, should_create_good_path )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_STREQ( test_db.type(), "mysql" );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db, should_execute_batch )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( batch ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db, should_fail_create_bad_con_str )
{
   EXPECT_THROW( ds::db::connection test_db( "mysql://blah?username=root" ), std::runtime_error );
   EXPECT_THROW( ds::db::connection test_db( "mysql://" ), std::invalid_argument );
   EXPECT_THROW( ds::db::connection test_db( "mysql://blah" ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_execute_simple )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   ds::db::statement create_test = test_db( create );
   EXPECT_NO_THROW( create_test.execute() );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_fail_create_bad_sql )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_THROW( test_db( bad_sql ), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_return_execute_value )
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

      EXPECT_NO_THROW( insert_test << data[ 0 ] );
      EXPECT_EQ( insert_test.execute(), 1 );

      EXPECT_NO_THROW( insert_test << data[ 1 ] );
      EXPECT_EQ( insert_test.execute(), 2 );

      ds::db::statement value_test = test_db( num_rows );

      EXPECT_EQ( value_test.execute(), 2 );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_execute_query_parameters )
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

      EXPECT_NO_THROW( insert_test << data[ 0 ] );
      EXPECT_NO_THROW( insert_test.execute() );

      EXPECT_NO_THROW( insert_test << 1 << 1 << 1 << 1 << 1
                                   << 1 << 1 << 1 << 1 << 1 << "hello2" );
      EXPECT_NO_THROW( insert_test.execute() );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_fail_query_too_many_parameters )
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

TEST( mysql_db_statement, should_fail_query_not_enough_parameters )
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

      EXPECT_THROW( insert_test.execute(), std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_provide_query_result_row )
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
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::result result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data )
      {
         if ( result )
         {
            result.step();
         }
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_statement, should_fail_bad_query )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test1 = test_db( create );
      ds::db::statement create_test2 = test_db( create );

      EXPECT_NO_THROW( create_test1.execute() );

      EXPECT_THROW( create_test2.execute(), std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_result, should_provide_query_data )
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
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::result result;

      EXPECT_NO_THROW( result = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         if ( result )
         {
            EXPECT_NO_THROW( result >> o_db );
         }

         EXPECT_EQ( o, o_db );

         if ( result )
         {
            result.step();
         }
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_result, should_provide_rows_affected )
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

   ds::db::statement delete_test = test_db( del_rows );

   EXPECT_EQ( delete_test.result().rows_affected(), 2 );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_result, should_return_query_data_not_available )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   ds::db::statement create_test = test_db( create );

   EXPECT_EQ( ds::db::result(), create_test.result() );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_db_result, should_fail_query_wrong_column_count )
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
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::result row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         if ( row )
         {
            EXPECT_NO_THROW( row >> o_db );
            EXPECT_THROW( row >> o_db, std::runtime_error );
         }

         EXPECT_EQ( o, o_db );

         if ( row )
         {
            row.step();
         }
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
