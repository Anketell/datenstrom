//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/test/test_db.h>

//-----------------------------------------------------------------------------

TEST( db_list_iterator, should_iterate_over_results )
{
   ds::db::statement stmt( std::make_shared< Test_statement >( 1, 5 ) );

   auto begin = stmt.begin();
   auto end   = stmt.end();

   int count = std::distance( begin, end );

   EXPECT_EQ( 5, count );
   EXPECT_EQ( begin->rows_affected(), count );
}

//-----------------------------------------------------------------------------

TEST( db_list_iterator, should_fail_dereferrence_end )
{
   ds::db::statement stmt( std::make_shared< Test_statement >( 0 ) );

   EXPECT_THROW( *stmt.end(), std::runtime_error );
}

//-----------------------------------------------------------------------------
