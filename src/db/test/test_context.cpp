//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/context.h>
#include <db/transaction.h>
#include <db/connect_string.h>
#include <dsutil/filesys.h>
#include <algorithm>
#include <array>

//-----------------------------------------------------------------------------

class Context : public testing::TestWithParam< const char * > {};

//-----------------------------------------------------------------------------

TEST_P( Context, should_construct_context )
{
   const char * con_string = GetParam();

   ds::db::context::enroll_sql_path_list( "." );

   EXPECT_NO_THROW( ds::db::context ctx( con_string ) );
   EXPECT_NO_THROW( { ds::db::connection con( con_string );  ds::db::context ctx( con ); } );
   EXPECT_NO_THROW( ds::db::context ctx( ds::db::parse_connect_string( con_string ) ) );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_insert_test_text )
{
   const char            * con_string   = GetParam();
   static constexpr char   database[]   = "test_db";
   static constexpr char * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

   ds::db::context::enroll_sql_path_list( "." );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_NO_THROW( ctx.execute_batch( "test.create_schema" ) );

   std::array< int, sizeof( text_table ) / sizeof( text_table[ 0 ] ) > ids = {};

   {
      auto insert = ctx( "test_table.insert", { "text" } );

      ds::db::transaction txn( ctx );

      int i = 0;
      for ( auto text : text_table )
         ids[ i++ ] = static_cast< int >( insert( text ).execute() );
   }

   EXPECT_EQ( ctx( "test.count_test_rows" ).execute(),
              sizeof( text_table ) / sizeof( text_table[ 0 ] ) );

   EXPECT_NO_THROW( ctx.drop( database ) );

   ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

TEST_P( Context, should_read_test_text )
{
   const char            * con_string = GetParam();
   static constexpr char   database[] = "test_db";
   static constexpr char * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

    ds::db::context::enroll_sql_path_list( "." );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_NO_THROW( ctx.execute_batch( "test.create_schema" ) );

   std::array< int, sizeof( text_table ) / sizeof( text_table[ 0 ] ) > ids = {};

   {
      auto insert = ctx( "test_table.insert", { "text" } );

      ds::db::transaction txn( ctx );

      int i = 0;
      for ( auto text : text_table )
         ids[ i++ ] = static_cast< int >( insert( text ).execute() );
   }

   EXPECT_EQ( ctx( "test.count_test_rows" ).execute(),
              sizeof( text_table ) / sizeof( text_table[ 0 ] ) );

   {
      auto query_text_by_id = ctx( "test_table.query_text_by_id" );

      ds::db::transaction txn( ctx );

      for ( int id : ctx( "test_table.list_id" ) )
      {
         auto it = std::find( ids.begin(), ids.end(), id );
         ASSERT_NE( it, ids.end() );

         std::string db_text = query_text_by_id( id ).result();

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

   auto params = ds::db::parse_connect_string( con_string );

    ds::db::context::clean_up();

   EXPECT_THROW_VALUE( ds::db::context ctx( params ),
                       ds::db::context::unsupported_db_type,
                       e.type == params[ "type" ] );
}


//-----------------------------------------------------------------------------

TEST_P( Context, should_fail_unknown_sql_key )
{
   const char            * con_string   = GetParam();
   static constexpr char   database[]   = "test_db";
   static constexpr char * text_table[] =
   {
      { "1st Text String" },
      { "2nd Text String" },
      { "3rd Text String" }
   };

    ds::db::context::enroll_sql_path_list( "." );

   ds::db::context ctx( con_string );

   EXPECT_NO_THROW( ctx.drop( database ) );
   EXPECT_NO_THROW( ctx.create( database ) );
   EXPECT_THROW_VALUE( ctx.execute_batch( "test.unknown" ),
                       ds::db::context::unknown_sql,
                       e.key == "test.unknown" );

    ds::db::context::clean_up();
}

//-----------------------------------------------------------------------------

static std::string sqlite_con_str = "sqlite:///" + ds::util::filesys::temp_directory();

INSTANTIATE_NAMESPACE_TEST_SUITE_P( db,
                                    sqlite,
                                    Context,
                                    testing::Values( sqlite_con_str.c_str() ) );

//-----------------------------------------------------------------------------
