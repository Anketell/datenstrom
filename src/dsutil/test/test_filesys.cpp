//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/filesys.h>
#include <fstream>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, filesys, should_detect_file_existance )
{
   EXPECT_TRUE( ds::util::filesys::exists( "../CMakeFiles" ) );
   EXPECT_FALSE( ds::util::filesys::exists( "/non_existent" ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, filesys, should_remove_file )
{
   static constexpr char file_name[] = "Unusual_file_name";

   EXPECT_FALSE( ds::util::filesys::exists( file_name ) );

   {
      std::ofstream( file_name ) << std::endl;
   }
   EXPECT_TRUE( ds::util::filesys::exists( file_name ) );

   ds::util::filesys::remove( file_name );

   EXPECT_FALSE( ds::util::filesys::exists( file_name ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, filesys, should_find_files )
{
   ds::util::filesys::find file_list( ".", { "*" } );
   EXPECT_GT( std::distance( file_list.begin(), file_list.end() ), 0 );
}

//-----------------------------------------------------------------------------
