//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <sstream>
#include <test_model/object.h>
#include <test_model/object_serialise.h>
#include <csv/csv.h>

//-----------------------------------------------------------------------------

static const char line[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,"Hello","2020-05-13","13:05:20","2020-05-13 13:05:20"
)";

static const char unquoted_strings[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,Hello,2020-05-13,13:05:20,2020-05-13 13:05:20
)";

static const char whitespace[] =
R"( -8 , -16 , -32 , -64 , 8 , 16 , 32 , 64 , 12.34 , 56.78 , "Hello" , "2020-05-13" , "13:05:20" , "2020-05-13 13:05:20"
)";

static const char embedded_quotes[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,"Hello ""you""","2020-05-13","13:05:20","2020-05-13 13:05:20"
)";

static const char extra_field[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,"Hello","2020-05-13","13:05:20","2020-05-13 13:05:20","ignored"
)";

static const char missing_separator[] =
R"(-8,-16,-32,-64,8,16,32 64,12.34,56.78,"Hello","2020-05-13","13:05:20","2020-05-13 13:05:20"
)";

static const char unterminated_quotes[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,"Hello","2020-05-13","13:05:20","2020-05-13 13:05:20
)";

static const char multiline[] =
R"(-8,-16,-32,-64,8,16,32,64,12.34,56.78,"He\
llo","2020-05-13","13:05:20","2020-05-13 13:05:20"
)";

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_read_comma_separated_values )
{
   std::stringstream ss( line );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ(      -8,               o.m_i8    );
   EXPECT_EQ(     -16,               o.m_i16   );
   EXPECT_EQ(     -32,               o.m_i32   );
   EXPECT_EQ(     -64,               o.m_i64   );
   EXPECT_EQ(       8,               o.m_u8    );
   EXPECT_EQ(      16,               o.m_u16   );
   EXPECT_EQ(      32,               o.m_u32   );
   EXPECT_EQ(      64,               o.m_u64   );
   EXPECT_EQ(   12.34,               o.m_f     );
   EXPECT_EQ(   56.78,               o.m_d     );
   EXPECT_EQ( "Hello",               o.m_hello );
   EXPECT_EQ( "2020-05-13",          o.m_date  );
   EXPECT_EQ( "13:05:20",            o.m_time  );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_skip_end_of_record )
{
   std::stringstream ss( line );

   test::Object o;

   ds::csv::istream in( ss );

   EXPECT_NO_THROW( in >> ds::endr );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_read_unquoted_strings )
{
  std::stringstream ss( unquoted_strings );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ(      -8,               o.m_i8    );
   EXPECT_EQ(     -16,               o.m_i16   );
   EXPECT_EQ(     -32,               o.m_i32   );
   EXPECT_EQ(     -64,               o.m_i64   );
   EXPECT_EQ(       8,               o.m_u8    );
   EXPECT_EQ(      16,               o.m_u16   );
   EXPECT_EQ(      32,               o.m_u32   );
   EXPECT_EQ(      64,               o.m_u64   );
   EXPECT_EQ(   12.34,               o.m_f     );
   EXPECT_EQ(   56.78,               o.m_d     );
   EXPECT_EQ( "Hello",               o.m_hello );
   EXPECT_EQ( "2020-05-13",          o.m_date  );
   EXPECT_EQ( "13:05:20",            o.m_time  );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_ignore_whitespace )
{
     std::stringstream ss( whitespace );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ(      -8,               o.m_i8    );
   EXPECT_EQ(     -16,               o.m_i16   );
   EXPECT_EQ(     -32,               o.m_i32   );
   EXPECT_EQ(     -64,               o.m_i64   );
   EXPECT_EQ(       8,               o.m_u8    );
   EXPECT_EQ(      16,               o.m_u16   );
   EXPECT_EQ(      32,               o.m_u32   );
   EXPECT_EQ(      64,               o.m_u64   );
   EXPECT_EQ(   12.34,               o.m_f     );
   EXPECT_EQ(   56.78,               o.m_d     );
   EXPECT_EQ( "Hello",               o.m_hello );
   EXPECT_EQ( "2020-05-13",          o.m_date  );
   EXPECT_EQ( "13:05:20",            o.m_time  );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_support_embeded_quotes )
{
   std::stringstream ss( embedded_quotes );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ(      -8,               o.m_i8    );
   EXPECT_EQ(     -16,               o.m_i16   );
   EXPECT_EQ(     -32,               o.m_i32   );
   EXPECT_EQ(     -64,               o.m_i64   );
   EXPECT_EQ(       8,               o.m_u8    );
   EXPECT_EQ(      16,               o.m_u16   );
   EXPECT_EQ(      32,               o.m_u32   );
   EXPECT_EQ(      64,               o.m_u64   );
   EXPECT_EQ(   12.34,               o.m_f     );
   EXPECT_EQ(   56.78,               o.m_d     );
   EXPECT_EQ( "Hello \"you\"",       o.m_hello );
   EXPECT_EQ( "2020-05-13",          o.m_date  );
   EXPECT_EQ( "13:05:20",            o.m_time  );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_ignore_extra_fields )
{
   std::stringstream ss( extra_field );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ(      -8,               o.m_i8    );
   EXPECT_EQ(     -16,               o.m_i16   );
   EXPECT_EQ(     -32,               o.m_i32   );
   EXPECT_EQ(     -64,               o.m_i64   );
   EXPECT_EQ(       8,               o.m_u8    );
   EXPECT_EQ(      16,               o.m_u16   );
   EXPECT_EQ(      32,               o.m_u32   );
   EXPECT_EQ(      64,               o.m_u64   );
   EXPECT_EQ(   12.34,               o.m_f     );
   EXPECT_EQ(   56.78,               o.m_d     );
   EXPECT_EQ( "Hello",               o.m_hello );
   EXPECT_EQ( "2020-05-13",          o.m_date  );
   EXPECT_EQ( "13:05:20",            o.m_time  );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime  );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_fail_missing_separator )
{
   std::stringstream ss( missing_separator );

   test::Object o;

   ds::csv::istream in( ss );

   EXPECT_THROW( in >> o >> ds::endr, ds::csv::Field_separator_expected );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_fail_unterminated_quotes )
{
   std::stringstream ss( unterminated_quotes );

   test::Object o;

   ds::csv::istream in( ss );

   EXPECT_THROW( in >> o >> ds::endr, ds::csv::Unterminated_quotes );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_fail_quotes_in_unquoted_strings )
{
   std::stringstream ss( "-8,-16,-32,-64,8,16,32,64,12.34,56.78,Hello\"\n" );

   test::Object o;

   ds::csv::istream in( ss );

   EXPECT_THROW( in >> o >> ds::endr, ds::csv::Quotes_not_allowed );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_fail_stream_underrun )
{
   std::stringstream ss;

   ss << line << std::endl << std::endl;

   test::Object o;

   ds::csv::istream in( ss );

   in >> o;

   EXPECT_EQ( -8, o.m_i8 );
   EXPECT_EQ( -16, o.m_i16 );
   EXPECT_EQ( -32, o.m_i32 );
   EXPECT_EQ( -64, o.m_i64 );
   EXPECT_EQ( 8, o.m_u8 );
   EXPECT_EQ( 16, o.m_u16 );
   EXPECT_EQ( 32, o.m_u32 );
   EXPECT_EQ( 64, o.m_u64 );
   EXPECT_EQ( 12.34, o.m_f );
   EXPECT_EQ( 56.78, o.m_d );
   EXPECT_EQ( "Hello", o.m_hello );
   EXPECT_EQ( "2020-05-13", o.m_date );
   EXPECT_EQ( "13:05:20", o.m_time );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime );

   EXPECT_THROW( in >> o, ds::stream_underrun );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, istream, should_read_multiline_string )
{
   std::stringstream ss( multiline );

   test::Object o;

   ds::csv::istream in( ss );

   in >> o >> ds::endr;

   EXPECT_EQ( -8, o.m_i8 );
   EXPECT_EQ( -16, o.m_i16 );
   EXPECT_EQ( -32, o.m_i32 );
   EXPECT_EQ( -64, o.m_i64 );
   EXPECT_EQ( 8, o.m_u8 );
   EXPECT_EQ( 16, o.m_u16 );
   EXPECT_EQ( 32, o.m_u32 );
   EXPECT_EQ( 64, o.m_u64 );
   EXPECT_EQ( 12.34, o.m_f );
   EXPECT_EQ( 56.78, o.m_d );
   EXPECT_EQ( "He\nllo", o.m_hello );
   EXPECT_EQ( "2020-05-13", o.m_date );
   EXPECT_EQ( "13:05:20", o.m_time );
   EXPECT_EQ( "2020-05-13 13:05:20", o.m_datetime );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( csv, ostream, should_write_comma_separated_values )
{
   std::stringstream ss;

   test::Object o =
   {
      -8, -16, -32, -64, 8, 16, 32, 64, 12.34, 56.78,
      "Hello \"you\"", "2020-05-13", "13:05:20", "2020-05-13 13:05:20"
   };

   ds::csv::ostream out( ss );

   out << o << ds::endr;

   EXPECT_EQ( embedded_quotes, ss.str() );
}

//-----------------------------------------------------------------------------
