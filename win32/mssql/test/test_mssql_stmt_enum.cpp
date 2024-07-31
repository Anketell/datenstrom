//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <mssql/test/mssql_test_data.h>
#include <mssql/statement_enum.h>
#include <algorithm>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mssql, stmt_enum, should_separate_statements )
{
   ds::mssql::statement_enum statements( batch );

   EXPECT_EQ( std::distance( statements.begin(), statements.end() ), 12 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( mssql, stmt_enum, should_remove_whitespace )
{
   ds::mssql::statement_enum statements( batch );

   for ( auto stmt : statements )
   {
      ASSERT_NE( *stmt.from, 0 );
      EXPECT_FALSE( std::isspace( *stmt.from ) );
      EXPECT_FALSE( *stmt.from == '-' && *( stmt.from + 1 ) == '-' );
      EXPECT_FALSE( *stmt.from == '/' && *( stmt.from + 1 ) == '*' );
   }
}

//-----------------------------------------------------------------------------
