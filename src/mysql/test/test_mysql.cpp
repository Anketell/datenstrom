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

//-----------------------------------------------------------------------------

const test_config_t test_config =
{ 
   MYSQL_TEST, 
   DS_MODULE_PATH, 
   SQL_MODULE_PATH 
};

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    Connection,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    Context,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    Statement,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    RowSet,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    BLOB,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    List,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    Parameter,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    Transaction,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    SavePoint,
                                    testing::Values( &test_config ) );


//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    mysql,
                                    StdSerial,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, Connection, should_return_type )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   ds::db::connection test_db( MYSQL_TEST );

   EXPECT_STREQ( test_db.type(), "mysql" );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mysql, Connection, should_fail_create_bad_path )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   EXPECT_THROW( ds::db::connection test_db( "mysql://" ), std::invalid_argument );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------
