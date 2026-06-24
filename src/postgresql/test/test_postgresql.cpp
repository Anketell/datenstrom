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
   POSTGRESQL_TEST,
   DS_MODULE_PATH,
   SQL_MODULE_PATH
};

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    Connection,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    Context,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    Statement,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    RowSet,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    BLOB,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    List,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    Parameter,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    Transaction,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common,
                                    postgresql,
                                    SavePoint,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( common, 
                                    postgresql,
                                    StdSerial,
                                    testing::Values( &test_config ) );

//-----------------------------------------------------------------------------

NAMESPACE_TEST( postgresql, Connection, should_return_type )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   ds::db::connection test_db( POSTGRESQL_TEST );

   EXPECT_STREQ( test_db.type(), "postgresql" );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( postgresql, Connection, should_fail_create_bad_path )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );

   EXPECT_THROW( ds::db::connection test_db( "postgresql://" ), std::invalid_argument );

   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( postgresql, Statement, should_execute_convert_integer_parameters )
{
   ds::db::connection::enroll_db_path_list( DS_MODULE_PATH );
   ds::db::context::enroll_sql_path_list( SQL_MODULE_PATH );

   ds::db::context test_db( POSTGRESQL_TEST );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << static_cast< int8_t >( 1 )
                                   << "hello2" << "2020-05-14"
                                   << "14:05:20" << "2020-05-14 14:05:20"
                                   << ds::endr );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );

   ds::db::context::clear_sql_path_list();
   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------
