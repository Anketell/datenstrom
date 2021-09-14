//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <db/transaction.h>
#include <mysql/test/mysql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( mysql_transaction, should_commit_on_destruction )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count = results_test.execute();

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_transaction, should_rollback_on_exception )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   try
   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );

      ds::db::statement results_test = test_db( num_rows );

      int count = results_test.execute();

      EXPECT_EQ( 2, count );

      throw 0;
   }
   catch ( ... )
   {
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count = results_test.execute();

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_release_on_destruction )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         ds::db::savepoint save( test_db, "save" );

         EXPECT_NO_THROW( insert_test << o << ds::endr );
      }
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count = results_test.execute();

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_rollback_on_exception )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( create ) );

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         try
         {
            ds::db::savepoint save( test_db, "save" );

            EXPECT_NO_THROW( insert_test << o << ds::endr );

            ds::db::statement results_test = test_db( num_rows );

            int count = results_test.execute();

            EXPECT_EQ( 1, count );

            throw 0;
         }
         catch ( ... )
         {
         }
      }
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count = results_test.execute();

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_fail_no_name )
{
   ds::db::connection test_db( test_con_str );

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

TEST( mysql_savepoint, should_fail_bad_release_name )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::transaction txn( test_db );
      ds::db::savepoint save( test_db, "save" );

      EXPECT_THROW( test_db.release_savepoint( "bad" ),  std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_fail_bad_rollback_name )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::transaction txn( test_db );
      ds::db::savepoint save( test_db, "save" );

      EXPECT_THROW( test_db.rollback_to_savepoint( "bad" ),  std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
