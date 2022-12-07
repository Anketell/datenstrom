//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/connection.h>
#include <db/transaction.h>
#include <mssql/test/mssql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mssql, list, should_provide_iterator )
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

   {
      int i = 0;
      for ( Object o : test_db( results ) )
         EXPECT_EQ( o, data[ i++ ] );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mssql, list, should_manage_cursor_within_transaction )
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

   {
      ds::db::transaction txn( test_db );
      int i = 0;

      ds::db::statement list = test_db( results );
      for ( Object o : list )
      {
         EXPECT_EQ( o, data[ i++ ] );
         break;
      }

      i = 0;
      for ( Object o : list )
      {
         EXPECT_EQ( o, data[ i++ ] );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mssql, list, should_manage_cursor_without_transaction )
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

   {
      int i = 0;

      ds::db::statement list = test_db( results );
      for ( Object o : list )
      {
         EXPECT_EQ( o, data[ i++ ] );
         break;
      }

      i = 0;
      for ( Object o : list )
      {
         EXPECT_EQ( o, data[ i++ ] );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
