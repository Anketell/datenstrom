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
   MYSQL_TEST, 
   DS_MODULE_PATH, 
   SQL_MODULE_PATH 
};

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          Connection,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          Context,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          Statement,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          RowSet,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          BLOB,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          List,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          Parameter,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          Transaction,
                          testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_TEST_SUITE_P( mysql,
                          SavePoint,
                          testing::Values( &test_config ) );


//-----------------------------------------------------------------------------
