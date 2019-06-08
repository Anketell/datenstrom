//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/test/test_db.h>

//-----------------------------------------------------------------------------

constexpr char Test_db::TYPE[];

//-----------------------------------------------------------------------------

TEST( db_endr, should_call_db_row_step )
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
   auto test_row = std::make_shared< Test_row >();
   ds::db::row row( test_row );

   EXPECT_EQ( test_row->m_step_calls, 0 );

   row >> ds::endr;

   EXPECT_EQ( test_row->m_step_calls, 1 );
}

//-----------------------------------------------------------------------------
