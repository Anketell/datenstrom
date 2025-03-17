//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/context.h>
#include <db/transaction.h>
#include <test_model/object.h>
#include <db/test/test_mod_savepoint.h>
#include <db/test/test_mod_config.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST_P( SavePoint, should_release_on_destruction )
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
      {
         ds::db::savepoint save( test_db, "savepoint" );

         EXPECT_NO_THROW( insert_test << o << ds::endr );
      }
   }

   {
      ds::db::statement results_test = test_db( "test.num_rows" );

      int count = results_test.result();

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( SavePoint, should_rollback_on_exception )
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
      {
         try
         {
            ds::db::savepoint save( test_db, "savepoint" );

            EXPECT_NO_THROW( insert_test << o << ds::endr );

            throw 0;
         }
         catch ( int )
         {
         }
      }
   }

   {
      ds::db::statement results_test = test_db( "test.num_rows" );

      int count = results_test.result();

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( SavePoint, should_fail_no_name  )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::transaction txn( test_db );
      EXPECT_THROW( test_db.savepoint( "" ),  std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( SavePoint, should_fail_bad_rollback_name  )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::transaction txn( test_db );
      ds::db::savepoint save( test_db, "savepoint" );

      EXPECT_THROW( test_db.rollback_to_savepoint( "bad" ) ,  std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
