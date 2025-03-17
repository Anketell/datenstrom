//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/context.h>
#include <db/transaction.h>
#include <test_model/object.h>
#include <db/test/test_mod_list.h>
#include <db/test/test_mod_config.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST_P( List, should_provide_iterator )
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
      int i = 0;
      for ( Object o_db : test_db( "test.results" ) )
      {
         Object o = data[ i++ ];

         o_db.m_time     = canonical_time( o_db.m_time );
         o_db.m_datetime = canonical_datetime( o_db.m_datetime );

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( List, should_manage_cursor_within_transaction )
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
      ds::db::transaction txn( test_db );
      int i = 0;

      ds::db::statement list = test_db( "test.results" );
      for ( Object o_db : list )
      {
         Object o = data[ i++ ];

         o_db.m_time     = canonical_time( o_db.m_time );
         o_db.m_datetime = canonical_datetime( o_db.m_datetime );

         EXPECT_EQ( o, o_db );
         break;
      }

      i = 0;
      for ( Object o_db : list )
      {
         Object o = data[ i++ ];

         o_db.m_time     = canonical_time( o_db.m_time );
         o_db.m_datetime = canonical_datetime( o_db.m_datetime );

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( List, should_manage_cursor_without_transaction )
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
      int i = 0;

      ds::db::statement list = test_db( "test.results" );
      for ( Object o_db : list )
      {
         Object o = data[ i++ ];

         o_db.m_time     = canonical_time( o_db.m_time );
         o_db.m_datetime = canonical_datetime( o_db.m_datetime );

         EXPECT_EQ( o, o_db );
         break;
      }

      i = 0;
      for ( Object o_db : list )
      {
         Object o = data[ i++ ];

         o_db.m_time     = canonical_time( o_db.m_time );
         o_db.m_datetime = canonical_datetime( o_db.m_datetime );

         EXPECT_EQ( o, o_db );
      }
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
