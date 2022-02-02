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
   ds::util::filesys::find file_list( ".", { "*" } );

   EXPECT_TRUE( ds::util::filesys::exists( ( *file_list.begin() ).c_str() ) );
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

   int count = 0;
   for ( auto f : file_list )
      count++;

   EXPECT_GT( count, 0 );
}

//-----------------------------------------------------------------------------
