//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/test/test_context.h>
#include <test_utils/gtest.h>
#include <db/context.h>
#include <db/transaction.h>
#include <dsutil/connect_string.h>
#include <dsutil/filesys.h>
#include <dsutil/env.h>
#include <algorithm>
#include <array>

//-----------------------------------------------------------------------------

Context::Context( void )
{
   m_sql_module_path = ds::env::get( "SQL_MODULE_PATH" );
   if ( m_sql_module_path.empty() )
      m_sql_module_path = ".";
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_construct_context )
{
   const char * con_string = GetParam();

   ds::db::context::enroll_sql_path_list( m_sql_module_path );

   EXPECT_NO_THROW( ds::db::context ctx( con_string ) );
   EXPECT_NO_THROW( { ds::db::connection con( con_string );  ds::db::context ctx( con ); } );
   EXPECT_NO_THROW( ds::db::context ctx( ds::parse_connect_string( con_string ) ) );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_perform_lookup )
{
   const char * con_string = GetParam();

   ds::db::context::enroll_sql_path_list( m_sql_module_path );

   ds::db::context ctx( con_string );

   EXPECT_STREQ( ctx.lookup( "test.type" ), ctx.type() );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_insert_test_text )
{
   const char            * con_string   = GetParam();
   static constexpr char   database[]   = "test_db";
   static const char     * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

   ds::db::context::enroll_sql_path_list( m_sql_module_path );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_NO_THROW( ctx.use( database ) );
   EXPECT_NO_THROW( ctx.execute_batch( "test.create_schema" ) );

   std::array< int, sizeof( text_table ) / sizeof( text_table[ 0 ] ) > ids = {};

   {
      auto insert = ctx( "test_table.insert", { "text" } );

      ds::db::transaction txn( ctx );

      int i = 0;
      for ( auto text : text_table )
         ids[ i++ ] = insert( text ).result();
   }

   EXPECT_EQ( static_cast< int >( ctx( "test.count_test_rows" ).result() ),
              sizeof( text_table ) / sizeof( text_table[ 0 ] ) );

   EXPECT_NO_THROW( ctx.drop( database ) );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_read_test_text )
{
   const char            * con_string = GetParam();
   static constexpr char   database[] = "test_db";
   static const char     * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

    ds::db::context::enroll_sql_path_list( m_sql_module_path );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_NO_THROW( ctx.use( database ) );
   EXPECT_NO_THROW( ctx.execute_batch( "test.create_schema" ) );

   std::array< int, sizeof( text_table ) / sizeof( text_table[ 0 ] ) > ids = {};

   {
      auto insert = ctx( "test_table.insert", { "text" } );

      ds::db::transaction txn( ctx );

      int i = 0;
      for ( auto text : text_table )
         ids[ i++ ] = insert( text ).result();
   }

   EXPECT_EQ( static_cast< int >( ctx( "test.count_test_rows" ).result() ),
              sizeof( text_table ) / sizeof( text_table[ 0 ] ) );

   {
      ds::db::transaction txn( ctx );

      for ( int id : ctx( "test_table.list_id" ) )
      {
         auto it = std::find( ids.begin(), ids.end(), id );
         ASSERT_NE( it, ids.end() );
      }

      auto query_text_by_id = ctx( "test_table.query_text_by_id", { "id" } );

      for ( int id : ids )
      {
         std::string db_text = query_text_by_id( id ).result();

         auto it = std::find( ids.begin(), ids.end(), id );
         std::string text( text_table[ it - ids.begin() ] );

         EXPECT_EQ( db_text, text );
      }
   }

   EXPECT_NO_THROW( ctx.drop( database ) );

    ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_fail_unsupported_database_type )
{
   const char * con_string = GetParam();

   auto params = ds::parse_connect_string( con_string );

   ds::db::context::clean_up();
   ds::db::context::enroll_sql_path_list( "" );

   EXPECT_NO_THROW( ds::db::connection con( params ) );

   EXPECT_THROW_VALUE( ds::db::context ctx( params ),
                       ds::db::context::unsupported_db_type,
                       e.type == params[ "type" ] );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_fail_unknown_sql_key )
{
   const char            * con_string   = GetParam();
   static constexpr char   database[]   = "test_db";
   static const     char * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

    ds::db::context::enroll_sql_path_list( m_sql_module_path );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_NO_THROW( ctx.use( database ) );
   EXPECT_THROW_VALUE( ctx.execute_batch( "test.unknown" ),
                       ds::db::context::unknown_sql,
                       e.key == "test.unknown" );

   EXPECT_NO_THROW( ctx.drop( database ) );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------
