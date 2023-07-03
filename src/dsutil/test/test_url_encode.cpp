//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/url_encode.h>

//-----------------------------------------------------------------------------

static std::string url     = " @#$%^&:;\n\"/`-_.!~*'()";
static std::string esc_url = "%20%40%23%24%25%5e%26%3a%3b%0a%22%2f%60-_.%21~%2a%27%28%29";
static std::string bad_esc = "%20%40%23%24%25%5e%26%3a%3b%a%22%2f%60-_.%21~%2a%27%28%29";

//-----------------------------------------------------------------------------

NAMESPACE_TEST( URL, Encode, should_escape_URL )
{
   std::string result = ds::url::escape( url );
   EXPECT_EQ( result, esc_url );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( URL, Decode, should_unescape_URL )
{
   std::string result = ds::url::unescape( esc_url );
   EXPECT_EQ( result, url );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( URL, Decode, should_recover_from_bad_URL_escape )
{
   std::string result = ds::url::unescape( bad_esc );
   EXPECT_EQ( result, url );
}

//-----------------------------------------------------------------------------
