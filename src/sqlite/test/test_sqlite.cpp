//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/test/test_mod_connection.h>
#include <db/test/test_mod_context.h>
#include <db/test/test_mod_statement.h>
#include <db/test/test_mod_rowset.h>
#include <db/test/test_mod_blob.h>
#include <db/test/test_mod_parameter.h>
#include <db/test/test_mod_transaction.h>
#include <db/test/test_mod_savepoint.h>
#include <db/test/test_mod_list.h>
#include <db/test/test_mod_config.h>
#include <db/test/test_mod_stdserial.h>
#include <test_utils/gtest.h>
#include <db/transaction.h>
#include <test_model/object.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

const test_config_t test_config =
{ 
   SQLITE_TEST, 
   DS_MODULE_PATH, 
   SQL_MODULE_PATH 
};

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    Connection,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    Context,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    Statement,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    RowSet,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    BLOB,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    List,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    Parameter,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    Transaction,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    SavePoint,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    sqlite,
                                    StdSerial,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, Connection, should_return_type )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   ds::db::connection test_db( SQLITE_TEST );

   EXPECT_STREQ( test_db.type(), "sqlite" );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, Connection, should_fail_create_bad_path )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   EXPECT_THROW( ds::db::connection test_db( "sqlite:///blah#test_db" ), std::runtime_error );
   EXPECT_THROW( ds::db::connection test_db( "sqlite://"              ), std::invalid_argument );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, RowSet, may_fail_query_wrong_column_type )
{
    ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
    ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

    ds::db::context test_db( SQLITE_TEST );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      ds::db::statement results_test = test_db( "test.results" );

      ds::db::rowset row;

      EXPECT_NO_THROW( row = results_test.result() );

      std::string hello;

      EXPECT_THROW( row >> hello, std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );

   ds::db::context::clear_sql_path_list();
   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, RowSet, should_query_min_max )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
   ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

   ds::db::context test_db( SQLITE_TEST );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      for ( auto o : data )
         EXPECT_NO_THROW( insert_test << o << ds::endr );
   }

   {
      static const char query[] = "SELECT MIN( hello ), MAX( hello ) FROM Object";

      ds::db::statement min_max_test = test_db.connection::operator()( query );

      std::string min;
      std::string max;

      ds::db::rowset result = min_max_test.result();

      result >> min 
             >> max;

      EXPECT_LT( min, max );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( sqlite, SavePoint, should_fail_bad_release_name  )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
   ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

   ds::db::connection test_db( SQLITE_TEST );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   {
      ds::db::transaction txn( test_db );
      ds::db::savepoint save( test_db, "mysavepoint" );

      EXPECT_THROW( test_db.release_savepoint( "bad" ) ,  std::runtime_error );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );

   ds::db::context::clear_sql_path_list();
   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------
