//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <mysql/connection.h>
#include <db/transaction.h>
#include <mysql/test/mysql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( mysql_transaction, should_commit_on_destruction )
{
   ds::mysql::connection test_db( test_server, username, password );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count;
      *results_test.begin() >> count;

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_transaction, should_rollback_on_exception )
{
   ds::mysql::connection test_db( test_server, username, password );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   try
   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }

      throw 0;
   }
   catch ( ... )
   {
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count;
      *results_test.begin() >> count;

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_release_on_destruction )
{
   ds::mysql::connection test_db( test_server, username, password );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         ds::db::savepoint save( test_db, "save" );

         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count;
      *results_test.begin() >> count;

      EXPECT_EQ( 2, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_rollback_on_exception )
{
   ds::mysql::connection test_db( test_server, username, password );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction( test_db );

      for ( auto o : data )
      {
         try
         {
            ds::db::savepoint save( test_db, "save" );

            EXPECT_NO_THROW( insert_test << o );
            EXPECT_NO_THROW( insert_test.execute() );

            throw 0;
         }
         catch ( ... )
         {
         }
      }
   }

   {
      ds::db::statement results_test = test_db( num_rows );

      int count;
      *results_test.begin() >> count;

      EXPECT_EQ( 0, count );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mysql_savepoint, should_fail_no_name )
{
   ds::mysql::connection test_db( test_server, username, password );

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
   ds::mysql::connection test_db( test_server, username, password );

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

TEST( mysql_savepoint, should_fail_bad_rollback_name
 )
{
   ds::mysql::connection test_db( test_server, username, password );

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
