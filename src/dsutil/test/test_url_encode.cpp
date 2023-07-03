//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <dsutil/url_encode.h>

//-----------------------------------------------------------------------------

static std::string url     = " @#$%^&:;\n\"/`-_.!~*'()";
static std::string esc_url = "%20%40%23%24%25%5e%26%3a%3b%0a%22%2f%60-_.%21~%2a%27%28%29";
static std::string bad_esc = "%20%40%23%24%25%5e%26%3a%3b%a%22%2f%60-_.%21~%2a%27%28%29";

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, URL, should_escape )
{
   std::string result = ds::url::escape( url );
   EXPECT_EQ( result, esc_url );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, URL, should_unescape )
{
   std::string result = ds::url::unescape( esc_url );
   EXPECT_EQ( result, url );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( util, URL, should_recover_unescaping_bad_encoding )
{
   std::string result = ds::url::unescape( bad_esc );
   EXPECT_EQ( result, url );
}

//-----------------------------------------------------------------------------
