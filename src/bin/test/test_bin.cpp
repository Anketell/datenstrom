//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <test_model/object.h>
#include <test_model/object_serialise.h>
#include <bin/bin.h>
#include <dsutil/endian.h>
#include <cstring>

//-----------------------------------------------------------------------------

class test_ostream : public ds::bin::ostream
{
public:

   test_ostream( std::streambuf * sb = nullptr ) : ds::bin::ostream( sb ) {}

   virtual ds::ostream & operator << ( const std::string & s ) override
   {
      static_cast< ds::ostream & >( *this ) << static_cast< uint32_t >( s.length() );
      m_sb->sputn( s.data(), s.length() );
      return *this;
   }
};

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, length, should_calculate_data_length )
{
   uint32_t data_0_length = sizeof( int8_t   ) +
                            sizeof( int16_t  ) +
                            sizeof( int32_t  ) +
                            sizeof( int64_t  ) +
                            sizeof( uint8_t  ) +
                            sizeof( uint16_t ) +
                            sizeof( uint32_t ) +
                            sizeof( uint64_t ) +
                            sizeof( double   ) +
                            sizeof( double   ) +
                            sizeof( uint32_t ) + data[ 0 ].m_hello.length() +
                            sizeof( uint32_t ) + data[ 0 ].m_date.length() +
                            sizeof( uint32_t ) + data[ 0 ].m_time.length() +
                            sizeof( uint32_t ) + data[ 0 ].m_datetime.length();

   EXPECT_EQ( ds::bin::length( ( uint8_t )0 ),  sizeof( uint8_t ) );
   EXPECT_EQ( ds::bin::length( ( uint16_t )0 ), sizeof( uint16_t ) );
   EXPECT_EQ( ds::bin::length( ( uint32_t )0 ), sizeof( uint32_t ) );
   EXPECT_EQ( ds::bin::length( ( uint64_t )0 ), sizeof( uint64_t ) );

   EXPECT_EQ( ds::bin::length( ( int8_t )0 ),  sizeof( int8_t ) );
   EXPECT_EQ( ds::bin::length( ( int16_t )0 ), sizeof( int16_t ) );
   EXPECT_EQ( ds::bin::length( ( int32_t )0 ), sizeof( int32_t ) );
   EXPECT_EQ( ds::bin::length( ( int64_t )0 ), sizeof( int64_t ) );

   EXPECT_EQ( ds::bin::length( 0.0 ), sizeof( double ) );

   EXPECT_THROW( ds::bin::length( data[ 0 ] ), ds::Not_implemented );
   EXPECT_EQ( ds::bin::length< test_ostream >( data[ 0 ] ), data_0_length );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u8_be )
{
   uint8_t u8       = 0x88;
   uint8_t buffer[] = { 0 };

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << u8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u8 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( buffer[ 0 ], u8 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u8_le )
{
   uint8_t u8       = 0x88;
   uint8_t buffer[] = { 0 };

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << u8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u8 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( buffer[ 0 ], u8 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u16_be )
{
   uint16_t u16      = 0x8899;
   uint8_t  buffer[] = { 0, 0 };

   auto beu16    = ds::endian::htob( u16 );
   auto expected = reinterpret_cast< uint8_t * >( &beu16 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << u16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u16 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u16_le )
{
   uint16_t u16      = 0x8899;
   uint8_t  buffer[] = { 0, 0 };

   auto leu16    = ds::endian::htol( u16 );
   auto expected = reinterpret_cast< uint8_t * >( &leu16 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << u16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u16 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u32_be )
{
   uint32_t u32      = 0x8899aabb;
   uint8_t  buffer[] = { 0, 0, 0, 0 };

   auto beu32    = ds::endian::htob( u32 );
   auto expected = reinterpret_cast< uint8_t * >( &beu32 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << u32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u32 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u32_le )
{
   uint32_t u32      = 0x8899aabb;
   uint8_t  buffer[] = { 0, 0, 0, 0 };

   auto leu32    = ds::endian::htol( u32 );
   auto expected = reinterpret_cast< uint8_t * >( &leu32 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << u32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u32 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u64_be )
{
   uint64_t u64      = 0x8899aabbccddeeffULL;
   uint8_t  buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto beu64    = ds::endian::htob( u64 );
   auto expected = reinterpret_cast< uint8_t * >( &beu64 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << u64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u64 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_u64_le )
{
   uint64_t u64      = 0x8899aabbccddeeffULL;
   uint8_t  buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto leu64    = ds::endian::htol( u64 );
   auto expected = reinterpret_cast< uint8_t * >( &leu64 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << u64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( u64 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s8_be )
{
   int8_t  s8       = -0x78;
   uint8_t buffer[] = { 0 };

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << s8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s8 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( buffer[ 0 ], static_cast< uint8_t >( s8 ) );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s8_le )
{
   int8_t  s8       = -0x78;
   uint8_t buffer[] = { 0 };

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << s8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s8 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( buffer[ 0 ], static_cast< uint8_t >( s8 )  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s16_be )
{
   int16_t s16      = -0x7899;
   uint8_t buffer[] = { 0, 0 };

   auto bes16    = ds::endian::htob( static_cast< uint16_t >( s16 ) );
   auto expected = reinterpret_cast< uint8_t * >( &bes16 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << s16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s16 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s16_le )
{
   int16_t s16      = -0x7899;
   uint8_t buffer[] = { 0, 0 };

   auto leu16    = ds::endian::htol( static_cast< uint16_t >( s16 ) );
   auto expected = reinterpret_cast< uint8_t * >( &leu16 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << s16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s16 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s32_be )
{
   int32_t s32      = -0x7899aabb;
   uint8_t buffer[] = { 0, 0, 0, 0 };

   auto bes32    = ds::endian::htob( static_cast< uint32_t >( s32 ) );
   auto expected = reinterpret_cast< uint8_t * >( &bes32 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << s32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s32 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s32_le )
{
   int32_t s32      = -0x7899aabb;
   uint8_t buffer[] = { 0, 0, 0, 0 };

   auto les32    = ds::endian::htol( static_cast< uint32_t >( s32 ) );
   auto expected = reinterpret_cast< uint8_t * >( &les32 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << s32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s32 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s64_be )
{
   int64_t s64      = -0x7899aabbccddeeffULL;
   uint8_t buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto bes64    = ds::endian::htob( static_cast< uint64_t >( s64 ) );
   auto expected = reinterpret_cast< uint8_t * >( &bes64 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << s64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s64 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_s64_le )
{
   int64_t s64      = -0x7899aabbccddeeffULL;
   uint8_t buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto les64    = ds::endian::htol( static_cast< uint64_t >( s64 ) );
   auto expected = reinterpret_cast< uint8_t * >( &les64 );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << s64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( s64 ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_double_be )
{
   double  d        = -123.456;
   uint8_t buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto expected = reinterpret_cast< uint8_t * >( &d );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::big_endian << d );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( d ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_put_double_le )
{
   double  d        = -123.456;
   uint8_t buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

   auto expected = reinterpret_cast< uint8_t * >( &d );

   ds::bin::streamwrap sw( buffer, sizeof( buffer ) );
   ds::bin::ostream    out( &sw );

   EXPECT_NO_THROW( out << ds::bin::little_endian << d );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.plength(), ds::bin::length( d ) );
   EXPECT_NE( sw.ppos(), sw.buffer() );
   EXPECT_EQ( sw.premaining(), 0 );

   EXPECT_EQ( memcmp( buffer, expected, sw.plength() ), 0 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u8_be )
{
   uint8_t u8       = 0;
   uint8_t buffer[] = { 0x88 };

   uint8_t expected = 0x88;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> u8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u8 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u8, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u8_le )
{
   uint8_t u8       = 0;
   uint8_t buffer[] = { 0x88 };

   uint8_t expected = 0x88;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> u8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u8 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u8, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u16_be )
{
   uint16_t u16      = 0;
   uint8_t  buffer[] = { 0x88, 0x99 };

   uint16_t expected = 0x8899;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> u16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u16 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u16, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u16_le )
{
   uint16_t u16      = 0;
   uint8_t  buffer[] = { 0x99, 0x88 };

   uint16_t expected = 0x8899;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> u16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u16 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u16, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u32_be )
{
   uint32_t u32      = 0;
   uint8_t  buffer[] = { 0x88, 0x99, 0xaa, 0xbb };

   uint32_t expected = 0x8899aabb;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> u32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u32 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u32, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u32_le )
{
   uint32_t u32      = 0;
   uint8_t  buffer[] = { 0xbb, 0xaa, 0x99, 0x88 };

   uint32_t expected = 0x8899aabb;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> u32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u32 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u32, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u64_be )
{
   uint64_t u64      = 0;
   uint8_t  buffer[] = { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

   uint64_t expected = 0x8899aabbccddeeffULL;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> u64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u64 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u64, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_u64_le )
{
   uint64_t u64      = 0;
   uint8_t  buffer[] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

   uint64_t expected = 0x8899aabbccddeeffULL;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> u64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( u64 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( u64, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s8_be )
{
   int8_t  s8       = 0;
   uint8_t buffer[] = { 0x88 };

   int8_t expected = 0x88;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> s8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s8 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s8, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s8_le )
{
   int8_t  s8       = 0;
   uint8_t buffer[] = { 0x88 };

   int8_t expected = 0x88;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> s8 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s8 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s8, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s16_be )
{
   int16_t s16      = 0;
   uint8_t buffer[] = { 0x88, 0x99 };

   int16_t expected = 0x8899;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> s16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s16 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s16, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s16_le )
{
   int16_t s16      = 0;
   uint8_t buffer[] = { 0x99, 0x88 };

   int16_t expected = 0x8899;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> s16 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s16 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s16, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s32_be )
{
   int32_t s32      = 0;
   uint8_t buffer[] = { 0x88, 0x99, 0xaa, 0xbb };

   int32_t expected = 0x8899aabb;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> s32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s32 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s32, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s32_le )
{
   int32_t s32      = 0;
   uint8_t buffer[] = { 0xbb, 0xaa, 0x99, 0x88 };

   int32_t expected = 0x8899aabb;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> s32 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s32 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s32, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s64_be )
{
   int64_t s64      = 0;
   uint8_t buffer[] = { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

   uint64_t expected = 0x8899aabbccddeeffULL;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> s64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s64 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s64, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_s64_le )
{
   int64_t s64      = 0;
   uint8_t buffer[] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

   uint64_t expected = 0x8899aabbccddeeffULL;

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> s64 );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( s64 ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( s64, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_double_be )
{
   double expected = -123.456;

   double  d           = 0.0;
   uint8_t buffer[ 8 ];

   memcpy( buffer, &expected, sizeof( expected ) );

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::big_endian >> d );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( d ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( d, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_get_double_le )
{
   double expected = -123.456;

   double  d           = 0.0;
   uint8_t buffer[ 8 ];

   memcpy( buffer, &expected, sizeof( expected ) );

   ds::bin::streambuf sw( sizeof( buffer ) );
   ds::bin::istream   in( &sw );

   sw.sputn( reinterpret_cast< char * >( buffer ), sizeof( buffer ) );

   EXPECT_NO_THROW( in >> ds::bin::little_endian >> d );

   EXPECT_EQ( sw.size(), sizeof( buffer ) );
   EXPECT_EQ( sw.glength(), ds::bin::length( d ) );
   EXPECT_NE( sw.gpos(), sw.buffer() );
   EXPECT_EQ( sw.gremaining(), 0 );

   EXPECT_EQ( d, expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, streamwrap, should_seek_buffer )
{
   ds::bin::streambuf sw( 100 );

   EXPECT_EQ( sw.glength(), 0 );
   EXPECT_EQ( sw.plength(), 0 );

   sw.pubseekoff( 10, std::ios::cur, std::ios::out );

   EXPECT_EQ( sw.glength(),  0 );
   EXPECT_EQ( sw.plength(), 10 );

   sw.pubseekoff( 10, std::ios::cur, std::ios::in );

   EXPECT_EQ( sw.glength(), 10 );
   EXPECT_EQ( sw.plength(), 10 );

   std::istream in( &sw );
   EXPECT_EQ( in.seekg( 1, std::ios::cur ).tellg(), 11 );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, ostream, should_fail_overrun )
{
   ds::bin::streamwrap sw( nullptr, 0 );
   ds::bin::ostream    out( &sw );

   EXPECT_THROW( out << ( uint8_t )1,  ds::stream_overrun );
   EXPECT_THROW( out << ( int8_t )1,   ds::stream_overrun );
   EXPECT_THROW( out << ( uint16_t )1, ds::stream_overrun );
   EXPECT_THROW( out << ( int16_t )1,  ds::stream_overrun );
   EXPECT_THROW( out << ( uint32_t )1, ds::stream_overrun );
   EXPECT_THROW( out << ( int32_t )1,  ds::stream_overrun );
   EXPECT_THROW( out << ( uint64_t )1, ds::stream_overrun );
   EXPECT_THROW( out << ( int64_t )1,  ds::stream_overrun );
   EXPECT_THROW( out << 1.0,           ds::stream_overrun );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( bin, istream, should_fail_underrun )
{
   ds::bin::streamwrap sw( nullptr, 0 );
   ds::bin::istream    in( &sw );

   EXPECT_THROW( { uint8_t  u8;  in >> u8;  }, ds::stream_underrun );
   EXPECT_THROW( { int8_t   s8;  in >> s8;  }, ds::stream_underrun );
   EXPECT_THROW( { uint16_t u16; in >> u16; }, ds::stream_underrun );
   EXPECT_THROW( { int16_t  s16; in >> s16; }, ds::stream_underrun );
   EXPECT_THROW( { uint32_t u32; in >> u32; }, ds::stream_underrun );
   EXPECT_THROW( { int32_t  s32; in >> s32; }, ds::stream_underrun );
   EXPECT_THROW( { uint64_t u64; in >> u64; }, ds::stream_underrun );
   EXPECT_THROW( { int64_t  s64; in >> s64; }, ds::stream_underrun );
   EXPECT_THROW( { double   d;   in >> d;   }, ds::stream_underrun );
}

//-----------------------------------------------------------------------------
