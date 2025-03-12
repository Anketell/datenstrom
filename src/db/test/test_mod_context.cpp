//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/test/test_mod_context.h>
#include <db/test/test_mod_config.h>
#include <test_utils/gtest.h>
#include <db/context.h>
#include <db/transaction.h>
#include <dsutil/connect_string.h>
#include <dsutil/filesys.h>
#include <dsutil/env.h>
#include <algorithm>
#include <array>

//-----------------------------------------------------------------------------

TEST_P( Context, should_construct_context )
{
   EXPECT_NO_THROW( ds::db::context ctx( config->constr ) );
   EXPECT_NO_THROW( { ds::db::connection con( config->constr );  ds::db::context ctx( con ); } );
   EXPECT_NO_THROW( ds::db::context ctx( ds::parse_connect_string( config->constr ) ) );
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_perform_lookup )
{
   ds::db::context ctx( config->constr );

   EXPECT_STREQ( ctx.lookup( "test.type" ), ctx.type() );
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_return_keys )
{
   const static ds::db::sql::keyset_t expected =
   {
      "test.bad_sql", 
      "test.batch", 
      "test.create", 
      "test.create_blob", 
      "test.del_rows", 
      "test.insert", 
      "test.insert_alt",
      "test.insert_blob", 
      "test.insert_id", 
      "test.named", 
      "test.no_result", 
      "test.num_rows", 
      "test.result", 
      "test.results", 
      "test.results_alt", 
      "test.select_blob", 
      "test.simple",
      "test.type"
   };

   ds::db::context ctx( config->constr );

   ds::db::sql::keyset_t keyset;

   EXPECT_NO_THROW( keyset = ctx.keys() );

   EXPECT_EQ( keyset, expected );
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_execute_batch )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.batch" ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_fail_unsupported_database_type )
{
   auto params = ds::parse_connect_string( config->constr );

   ds::db::context::clear_sql_path_list();
   ds::db::context::enroll_sql_path_list( "" );

   EXPECT_NO_THROW( ds::db::connection con( params ) );

   EXPECT_THROW_VALUE( ds::db::context ctx( params ),
                       ds::db::context::unsupported_db_type,
                       e.type == params[ "type" ] );
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_fail_unknown_sql_key )
{
   ds::db::context ctx( config->constr );

   EXPECT_NO_THROW( ctx.drop( test_db_name ) );
   EXPECT_NO_THROW( ctx.create( test_db_name ) );
   EXPECT_NO_THROW( ctx.use( test_db_name ) );
   EXPECT_THROW_VALUE( ctx.execute_batch( "test.unknown" ),
                       ds::db::context::unknown_sql,
                       e.key == "test.unknown" );

   EXPECT_NO_THROW( ctx.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
