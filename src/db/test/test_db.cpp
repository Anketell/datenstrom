//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/test/test_db.h>
#include <test_model/object.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, endr, should_call_db_statement_execute )
{
   auto test_stmt = std::make_shared< Test_statement >();
   ds::db::statement stmt( test_stmt );

   EXPECT_EQ( test_stmt->m_execute_calls, 0 );

   stmt << ds::endr;

   EXPECT_EQ( test_stmt->m_execute_calls, 1 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, endr, should_call_db_rowset_step )
{
   auto test_rowset = std::make_shared< Test_rowset >();
   ds::db::rowset rowset( test_rowset );

   EXPECT_EQ( test_rowset->m_step_calls, 0 );

   rowset >> ds::endr;

   EXPECT_EQ( test_rowset->m_step_calls, 1 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, cast, should_support_statement_cast_operator )
{
   auto test_stmt = std::make_shared< Test_statement >();
   ds::db::statement stmt( test_stmt );

   EXPECT_EQ( test_stmt->m_result_calls, 0 );

   test::Object o;

   o.m_i8       = stmt;
   o.m_i16      = stmt;
   o.m_i32      = stmt;
   o.m_i64      = stmt;
   o.m_u8       = stmt;
   o.m_u16      = stmt;
   o.m_u32      = stmt;
   o.m_u64      = stmt;
   o.m_f        = stmt;
   o.m_d        = stmt;
   o.m_hello    = stmt;
   o.m_date     = stmt;
   o.m_time     = stmt;
   o.m_datetime = stmt;

   EXPECT_EQ( test_stmt->m_result_calls, 14 );
}

//-----------------------------------------------------------------------------
