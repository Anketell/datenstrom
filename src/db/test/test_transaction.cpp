//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/test/test_db.h>
#include <db/transaction.h>

//-----------------------------------------------------------------------------

TEST( db_transaction, should_begin_transaction_upon_construction )
{
   Test_db db;

   EXPECT_EQ( db.m_begin_txn_calls,    0 );
   EXPECT_EQ( db.m_commit_txn_calls,   0 );
   EXPECT_EQ( db.m_rollback_txn_calls, 0 );

   ds::db::transaction transaction( db );

   EXPECT_EQ( db.m_begin_txn_calls,    1 );
   EXPECT_EQ( db.m_commit_txn_calls,   0 );
   EXPECT_EQ( db.m_rollback_txn_calls, 0 );
}

//-----------------------------------------------------------------------------

TEST( db_transaction, should_commit_transaction_upon_destruction )
{
   Test_db db;

   EXPECT_EQ( db.m_begin_txn_calls,    0 );
   EXPECT_EQ( db.m_commit_txn_calls,   0 );
   EXPECT_EQ( db.m_rollback_txn_calls, 0 );

   {
      ds::db::transaction transaction( db );
   }

   EXPECT_EQ( db.m_begin_txn_calls,    1 );
   EXPECT_EQ( db.m_commit_txn_calls,   1 );
   EXPECT_EQ( db.m_rollback_txn_calls, 0 );
}

//-----------------------------------------------------------------------------

TEST( db_transaction, should_rollback_transaction_upon_exception )
{
   Test_db db;

   EXPECT_EQ( db.m_begin_txn_calls,    0 );
   EXPECT_EQ( db.m_commit_txn_calls,   0 );
   EXPECT_EQ( db.m_rollback_txn_calls, 0 );

   try
   {
      ds::db::transaction transaction( db );

      throw 0;
   }
   catch ( ... )
   {
   }

   EXPECT_EQ( db.m_begin_txn_calls,    1 );
   EXPECT_EQ( db.m_commit_txn_calls,   0 );
   EXPECT_EQ( db.m_rollback_txn_calls, 1 );
}

//-----------------------------------------------------------------------------

TEST( db_savepoint, should_create_savepoint_upon_construction )
{
   Test_db db;

   EXPECT_EQ( db.m_savepoint_calls,             0 );
   EXPECT_EQ( db.m_release_savepoint_calls,     0 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 0 );

   ds::db::transaction transaction( db );

   ds::db::savepoint savepoint( db, "save" );

   EXPECT_EQ( db.m_savepoint_calls,             1 );
   EXPECT_EQ( db.m_release_savepoint_calls,     0 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 0 );
}

//-----------------------------------------------------------------------------

TEST( db_savepoint, should_release_savepoint_upon_destruction )
{
   Test_db db;

   EXPECT_EQ( db.m_savepoint_calls,             0 );
   EXPECT_EQ( db.m_release_savepoint_calls,     0 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 0 );

   ds::db::transaction transaction( db );

   {
      ds::db::savepoint savepoint( db, "save" );
   }

   EXPECT_EQ( db.m_savepoint_calls,             1 );
   EXPECT_EQ( db.m_release_savepoint_calls,     1 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 0 );
}

//-----------------------------------------------------------------------------

TEST( db_savepoint, should_rollback_savepoint_upon_exception )
{
   Test_db db;

   EXPECT_EQ( db.m_savepoint_calls,             0 );
   EXPECT_EQ( db.m_release_savepoint_calls,     0 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 0 );

   ds::db::transaction transaction( db );

   try
   {
      ds::db::savepoint savepoint( db, "save" );
      throw 0;
   }
   catch ( ... )
   {
   }

   EXPECT_EQ( db.m_savepoint_calls,             1 );
   EXPECT_EQ( db.m_release_savepoint_calls,     0 );
   EXPECT_EQ( db.m_rollback_to_savepoint_calls, 1 );
}

//-----------------------------------------------------------------------------
