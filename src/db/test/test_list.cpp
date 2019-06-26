//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/test/test_db.h>

//-----------------------------------------------------------------------------

TEST( db_list_iterator, should_iterate_over_results )
{
   ds::db::statement stmt( std::make_shared< Test_statement >( 5 ) );

   int count = std::distance( stmt.begin(), stmt.end() );

   EXPECT_EQ( 5, count );
}

//-----------------------------------------------------------------------------

TEST( db_list_iterator, should_fail_dereferrence_end )
{
   ds::db::statement stmt( std::make_shared< Test_statement >( 0 ) );

   EXPECT_THROW( *stmt.end(), std::runtime_error );
}

//-----------------------------------------------------------------------------
