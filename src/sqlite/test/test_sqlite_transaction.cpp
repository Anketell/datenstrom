//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <sqlite/connection.h>
#include <db/transaction.h>
#include <sqlite/test/sqlite_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( sqlite_transaction, should_commit_on_destruction )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

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

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
 }

//-----------------------------------------------------------------------------

TEST( sqlite_transaction, should_rollback_on_exception )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

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

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_transaction, should_fail_nested )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      ds::db::transaction transaction1( test_db );

      EXPECT_THROW( ds::db::transaction transaction2( test_db ),
                    std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_transaction, should_fail_lone_commit )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   EXPECT_THROW( test_db.commit_transaction(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_transaction, should_fail_lone_rollback )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   EXPECT_THROW( test_db.rollback_transaction(), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_savepoint, should_release_on_destruction )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

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

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_savepoint, should_rollback_on_exception )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

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

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_savepoint, should_fail_no_name  )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   EXPECT_THROW( test_db.savepoint( "" ) ,  std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_savepoint, should_fail_bad_release_name  )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   ds::db::savepoint save( test_db, "save" );

   EXPECT_THROW( test_db.release_savepoint( "bad" ) ,  std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_savepoint, should_fail_bad_rollback_name  )
{
   ds::sqlite::connection test_db( tmp_path );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
   EXPECT_NO_THROW( test_db.create( tmp_db ) );
   EXPECT_NO_THROW( test_db.use( tmp_db ) );

   ds::db::savepoint save( test_db, "save" );

   EXPECT_THROW( test_db.rollback_to_savepoint( "bad" ) ,  std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( tmp_db ) );
}

//-----------------------------------------------------------------------------
