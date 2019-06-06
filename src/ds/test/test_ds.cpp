//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <ds/ds.h>

//-----------------------------------------------------------------------------

template< typename T > void get( ds::istream &in )
{
   T t;
   in >> t;
}

//-----------------------------------------------------------------------------

template< typename T > void put( ds::ostream &out )
{
   T t;
   out << t;
}

//-----------------------------------------------------------------------------

struct test_istream : public ds::istream
{
   bool m_flag;
};

//-----------------------------------------------------------------------------

struct test_ostream : public ds::ostream
{
   bool m_flag;
};

//-----------------------------------------------------------------------------

ds::istream & manip( ds::istream & in )
{
   dynamic_cast< test_istream & >( in ).m_flag = true;
   return in;
}

//-----------------------------------------------------------------------------

ds::ostream & manip( ds::ostream & out )
{
   dynamic_cast< test_ostream & >( out ).m_flag = true;
   return out;
}

//-----------------------------------------------------------------------------

TEST( ds_serial_istream, should_fail_input )
{
   ds::istream in;

   EXPECT_THROW( get< int8_t >( in ),  ds::Not_implemented );
   EXPECT_THROW( get< int16_t >( in ), ds::Not_implemented );
   EXPECT_THROW( get< int32_t >( in ), ds::Not_implemented );
   EXPECT_THROW( get< int64_t >( in ), ds::Not_implemented );

   EXPECT_THROW( get< uint8_t >( in ),  ds::Not_implemented );
   EXPECT_THROW( get< uint16_t >( in ), ds::Not_implemented );
   EXPECT_THROW( get< uint32_t >( in ), ds::Not_implemented );
   EXPECT_THROW( get< uint64_t >( in ), ds::Not_implemented );

   EXPECT_THROW( get< double >( in ), ds::Not_implemented );

   EXPECT_THROW( get< std::string >( in ), ds::Not_implemented );

   EXPECT_THROW( in.read( nullptr, 0 ), ds::Not_implemented );
}

//-----------------------------------------------------------------------------

TEST( ds_serial_istream, should_run_serial_manip )
{
   test_istream in;

   in.m_flag = false;

   in >> manip;

   EXPECT_TRUE( in.m_flag );
}

//-----------------------------------------------------------------------------

TEST( ds_serial_ostream, should_fail_output )
{
   ds::ostream out;

   EXPECT_THROW( put< int8_t >( out ),  ds::Not_implemented );
   EXPECT_THROW( put< int16_t >( out ), ds::Not_implemented );
   EXPECT_THROW( put< int32_t >( out ), ds::Not_implemented );
   EXPECT_THROW( put< int64_t >( out ), ds::Not_implemented );

   EXPECT_THROW( put< uint8_t >( out ),  ds::Not_implemented );
   EXPECT_THROW( put< uint16_t >( out ), ds::Not_implemented );
   EXPECT_THROW( put< uint32_t >( out ), ds::Not_implemented );
   EXPECT_THROW( put< uint64_t >( out ), ds::Not_implemented );

   EXPECT_THROW( put< double >( out ), ds::Not_implemented );

   EXPECT_THROW( put< char * >( out ), ds::Not_implemented );
   EXPECT_THROW( put< std::string >( out ), ds::Not_implemented );

   EXPECT_THROW( out.write( nullptr, 0 ), ds::Not_implemented );
}

//-----------------------------------------------------------------------------

TEST( ds_serial_ostream, should_run_serial_manip )
{
   test_ostream out;

   out.m_flag = false;

   out << manip;

   EXPECT_TRUE( out.m_flag );
}

//-----------------------------------------------------------------------------
