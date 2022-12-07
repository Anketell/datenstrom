//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/env.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, env, should_retrieve_variable )
{
   std::string path = ds::env::get( "PATH" );
   EXPECT_FALSE( path.empty() );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, env, should_iterate_dir_list )
{
   std::string path = ds::env::get( "PATH" );

   EXPECT_FALSE( path.empty() );

   ds::env::dir_list dl( path );

   EXPECT_GT( std::distance( dl.begin(), dl.end() ), 1 );
}

//-----------------------------------------------------------------------------
