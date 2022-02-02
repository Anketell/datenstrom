//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/module.h>
#include <dsutil/filesys.h>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, module, should_load_module )
{
   ds::util::filesys::find module_files( ".", { "*null_module*" } );
   ds::util::module        module( *module_files.begin() );

   EXPECT_TRUE( static_cast< bool >( module ) );
   EXPECT_NE( module.symbol< const void * >( "enroll" ), nullptr );
}

//-----------------------------------------------------------------------------
