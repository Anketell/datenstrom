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

//-----------------------------------------------------------------------------

const test_config_t test_config =
{ 
   SQLITE_TEST, 
   DS_MODULE_PATH, 
   SQL_MODULE_PATH 
};

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          Connection,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          Context,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          Statement,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          RowSet,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          BLOB,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          List,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          Parameter,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          Transaction,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( sqlite,
                          SavePoint,
                          testing::Values( &test_config ) );


//-----------------------------------------------------------------------------

TEST( sqlite, should_return_type )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   ds::db::connection test_db( SQLITE_TEST );

   EXPECT_STREQ( test_db.type(), "sqlite" );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

TEST( sqlite, should_fail_create_bad_path )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   EXPECT_THROW( ds::db::connection test_db( "sqlite:///blah#test_db" ), std::runtime_error );
   EXPECT_THROW( ds::db::connection test_db( "sqlite://"              ), std::invalid_argument );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------
