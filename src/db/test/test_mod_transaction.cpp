//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/context.h>
#include <db/transaction.h>
#include <test_model/object.h>
#include <db/test/test_mod_transaction.h>
#include <db/test/test_mod_config.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST_P( Transaction, should_commit_on_destruction )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.num_rows" );

      int count = results_test.result();

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
 }

//-----------------------------------------------------------------------------

TEST_P( Transaction, should_rollback_on_exception )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   try
   {
      ds::db::statement insert_test = test_db( "test.insert" );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );

      throw 0;
   }
   catch ( ... )
   {
   }

   {
      ds::db::statement results_test = test_db( "test.num_rows" );

      int count = results_test.result();

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( Transaction, should_support_nested )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      ds::db::transaction transaction1( test_db );

      EXPECT_NO_THROW( ds::db::transaction transaction2( test_db ) );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( Transaction, should_fail_lone_commit )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   EXPECT_THROW( test_db.commit_transaction(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( Transaction, should_fail_lone_rollback )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   EXPECT_THROW( test_db.rollback_transaction(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
