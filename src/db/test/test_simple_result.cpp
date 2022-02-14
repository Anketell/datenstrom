//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/simple_result.h>
#include <limits>

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, simple_result, should_return_single_value )
{
   ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );

   EXPECT_EQ( res.column_count(), 1 );
   EXPECT_EQ( res.rows_affected(), 1 );

   EXPECT_EQ( static_cast< int >( res ), 123 );

   EXPECT_THROW_VALUE( static_cast< int >( res ), 
                       std::runtime_error, 
                       strcmp( e.what(), "Get simple result column: No column available" ) == 0 );

   EXPECT_EQ( res.eof(), false );
   EXPECT_EQ( res.step(), false );
   EXPECT_EQ( res.eof(), true );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, simple_result, should_return_typed_value )
{
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< int8_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< int16_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< int32_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< int64_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< uint8_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< uint16_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< uint32_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< uint64_t >( res ), 123 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
      EXPECT_EQ( static_cast< double >( res ), 123.0 );
   }
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, simple_result, should_error_value_too_big )
{
   typedef std::numeric_limits< uint64_t > uint64_limits;

   constexpr char message[] = "Get simple result column: Integer data value too big";

   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< int8_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< int16_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< int32_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< int64_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< uint8_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< uint16_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
   {
      ds::db::result res( std::make_shared< ds::db::simple_result >( uint64_limits::max() ) );

      EXPECT_THROW_VALUE( static_cast< uint32_t >( res ), 
                        std::runtime_error, 
                        strcmp( e.what(), message ) == 0 );
   }
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, simple_result, should_error_not_character_type )
{
   ds::db::result res( std::make_shared< ds::db::simple_result >( 123 ) );
   EXPECT_THROW_VALUE( res.operator std::string (), 
                     std::runtime_error, 
                     strcmp( e.what(), "Get simple result column: Not character type" ) == 0 );
}

//-----------------------------------------------------------------------------
