//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/test/test_context.h>
#include <test_utils/gtest.h>
#include <firebird/test/firebird_test_data.h>

//-----------------------------------------------------------------------------

INSTANTIATE_NAMESPACE_TEST_SUITE_P( firebird,
                                    SQL_Module,
                                    Context,
                                    testing::Values( test_con_str ) );

//-----------------------------------------------------------------------------
