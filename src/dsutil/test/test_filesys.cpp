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
   auto temp_file_name = ds::util::filesys::temp_directory() + "temp_file";

   {
      std::ofstream ofs( temp_file_name );
   }

   EXPECT_TRUE( ds::util::filesys::exists( temp_file_name.c_str() ) );
   EXPECT_FALSE( ds::util::filesys::exists( "/non_existent" ) );

   ds::util::filesys::remove( temp_file_name.c_str() );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, filesys, should_remove_file )
{
   auto temp_file_name = ds::util::filesys::temp_directory() + "temp_file";

  ds::util::filesys::remove( temp_file_name.c_str() );

   EXPECT_FALSE( ds::util::filesys::exists( temp_file_name.c_str() ) );

   {
      std::ofstream ofs( temp_file_name );
   }

   EXPECT_TRUE( ds::util::filesys::exists( temp_file_name.c_str() ) );

   ds::util::filesys::remove( temp_file_name.c_str() );

   EXPECT_FALSE( ds::util::filesys::exists( temp_file_name.c_str() ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, filesys, should_find_files )
{
   auto temp_file_name = ds::util::filesys::temp_directory() + "temp_file_";

   for ( int i = 0; i < 5; i++ )
      std::ofstream ofs( temp_file_name + std::to_string( i ) );

   ds::util::filesys::find file_list( ".", { "*" } );

   int count = 0;
   for ( auto f : file_list )
      count++;

   EXPECT_GE( count, 5 );

   for ( int i = 0; i < 5; i++ )
      ds::util::filesys::remove( ( temp_file_name + std::to_string( i ) ).c_str() );

}

//-----------------------------------------------------------------------------
