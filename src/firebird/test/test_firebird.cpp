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
   FIREBIRD_TEST, 
   DS_MODULE_PATH, 
   SQL_MODULE_PATH 
};

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    Connection,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    Context,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    Statement,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    RowSet,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    BLOB,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    List,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    Parameter,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    Transaction,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    SavePoint,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    firebird,
                                    StdSerial,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

NAMESPACE_TEST( firebird, Connection, should_return_type )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   ds::db::connection test_db( FIREBIRD_TEST );

   EXPECT_STREQ( test_db.type(), "firebird" );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( firebird, Connection, should_fail_create_bad_con_str )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   EXPECT_THROW( ds::db::connection test_db( "firebird://" ), std::invalid_argument );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( firebird, RowSet, should_fail_query_wrong_column_type )
{
    ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
    ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

    ds::db::context test_db( FIREBIRD_TEST );

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

NAMESPACE_TEST( firebird, SavePoint, should_fail_bad_release_name  )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
   ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

   ds::db::connection test_db( FIREBIRD_TEST );

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
