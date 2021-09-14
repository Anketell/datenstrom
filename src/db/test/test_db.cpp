//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/test/test_db.h>

//-----------------------------------------------------------------------------

constexpr char Test_db::TYPE[];

//-----------------------------------------------------------------------------

TEST( db_endr, should_call_db_result_step )
{
   auto test_stmt = std::make_shared< Test_statement >();
   ds::db::statement stmt( test_stmt );

   EXPECT_EQ( test_stmt->m_execute_calls, 0 );

   stmt << ds::endr;

   EXPECT_EQ( test_stmt->m_execute_calls, 1 );
}

//-----------------------------------------------------------------------------

TEST( db_endr, should_call_db_statement_execute )
{
   auto test_result = std::make_shared< Test_result >();
   ds::db::result result( test_result );

   EXPECT_EQ( test_result->m_step_calls, 0 );

   result >> ds::endr;

   EXPECT_EQ( test_result->m_step_calls, 1 );
}

//-----------------------------------------------------------------------------
