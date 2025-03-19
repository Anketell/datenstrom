//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/statement_enum.h>
#include <dsutil/string.h>

#include <functional>
#include <set>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

enum ctype_t
{
   null        = 0b00000000,
   control     = 0b00000001,
   digit       = 0b00000010,
   alpha       = 0b00000100,
   space       = 0b00001000,
   punctuation = 0b00010000,
   quotation   = 0b00100000
};

//-----------------------------------------------------------------------------

static uint8_t ctype[] =
{
   null,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   space,
   space,
   space,
   space,
   space,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   control,
   space,
   punctuation,
   quotation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   quotation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   digit,
   digit,
   digit,
   digit,
   digit,
   digit,
   digit,
   digit,
   digit,
   digit,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   alpha,
   punctuation,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   alpha,
   punctuation,
   punctuation,
   punctuation,
   punctuation,
   control
};

//-----------------------------------------------------------------------------

static const std::function< bool( int, int )> CmpNoCase = []( int c1, int c2 )
{
   return std::tolower( c1 ) == std::tolower( c2 );
};

//-----------------------------------------------------------------------------

struct insensitive_compare
{
   bool operator()( const std::string & s1, const std::string & s2 ) const
   {
      return ds::string::cmpignorecase( s1.c_str(), s2.c_str() ) < 0;
   }
};

//-----------------------------------------------------------------------------

typedef std::set< std::string, insensitive_compare > nocase_set_t;

//-----------------------------------------------------------------------------

statement_enum::statement_enum( const std::string & statements ) :
m_statements( statements )
{
}

//-----------------------------------------------------------------------------

statement_enum::iterator statement_enum::begin( void ) const
{
   return { m_statements };
}

//-----------------------------------------------------------------------------

statement_enum::iterator statement_enum::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

statement_enum::iterator::iterator( const std::string & statements ) :
m_statements( statements )
{
   m_statement.from = m_statements.c_str();
   m_statement.len  = 0;

   next_statement();
}

//-----------------------------------------------------------------------------

static const char * skip_eol( const char * c )
{
   while ( *c && *c != '\n' )
      c++;

   return c;
}

//-----------------------------------------------------------------------------

static const char * skip_block_comment( const char * c )
{
   while ( *c )
   {
      if ( *c == '*' && *( c + 1 ) == '/' )
      {
         c += 2;
         break;
      }
      c++;
   }

   return c;
}

//-----------------------------------------------------------------------------

static const char * skip_ws( const char * c )
{
   for ( ;; )
   {
      while ( *c && ( std::isspace( *c ) ) )
         c++;

      if ( *c == '-' && *( c + 1 ) == '-' )
      {
         c = skip_eol( c );
         continue;
      }

      if ( *c == '/' && *( c + 1 ) == '*' )
      {
         c = skip_block_comment( c );
         continue;
      }
      break;
   }

   return c;
}

//-----------------------------------------------------------------------------

static const char * skip_number( const char * c )
{
   while ( std::isdigit( *c ) )
      c++;

   return c;
}

//-----------------------------------------------------------------------------

static const char * skip_identifier( const char * c )
{
   static const uint8_t ident = digit | alpha;

   while ( ( ctype[ *c ] & ident ) != 0  )
      c++;

   return c;
}

//-----------------------------------------------------------------------------

static const char * skip_string( const char * c )
{
   char q = *c++;

   while ( *c != q )
      c++;

   return ++c;
}

//-----------------------------------------------------------------------------

statement_enum::iterator::token_t statement_enum::iterator::next_token( const char * c )
{
   token_t token = { skip_ws( c ), 0 };

   switch ( ctype[ *token.from  & 0x7f ] )
   {
      case null:
         break;

      case digit:
         token.len = skip_number( token.from ) - token.from;
         break;

      case alpha:
         token.len = skip_identifier( token.from ) - token.from;
         break;

      case punctuation:
         token.len++;
         break;

      case quotation:
         token.len = skip_string( token.from ) - token.from;
   }

   return token;
}

//-----------------------------------------------------------------------------

statement_enum::iterator::token_t statement_enum::iterator::skip_to_end_token( const char * from )
{
   using namespace string;

   static const std::string end = "end";

   token_t token = next_token( from );
   while ( *token.from )
   {
      if ( token.len == end.length() && cmpignorecase( token.from, end.c_str(), token.len ) == 0 )
         break;

      token = next_token( token.from + token.len );
   }

   return token;
}

//-----------------------------------------------------------------------------

void statement_enum::iterator::next_statement( void )
{
   using namespace string;

   static const std::string begin = "begin";

   if ( !m_statement.from )
      return;

   m_statement.from = skip_ws( m_statement.from + m_statement.len );

   token_t token = next_token( m_statement.from );
   while ( *token.from && *token.from != ';' )
   {
      if ( token.len == begin.length() && cmpignorecase( token.from, begin.c_str(), token.len ) == 0 )
         token = skip_to_end_token( token.from + token.len );

      token = next_token( token.from + token.len );
   }

   if ( !*m_statement.from )
   {
      m_statement.from = nullptr;
      m_statement.len  = 0;
      return;
   }

   m_statement.len = token.from + token.len - m_statement.from;
}

//-----------------------------------------------------------------------------

const statement_enum::iterator::statement_t & statement_enum::iterator::operator*( void ) const
{
   return m_statement;
}

//-----------------------------------------------------------------------------

const statement_enum::iterator::statement_t * statement_enum::iterator::operator->( void ) const
{
   return &m_statement;
}

//-----------------------------------------------------------------------------

statement_enum::iterator statement_enum::iterator::operator++( void )
{
   next_statement();
   return *this;
}

//-----------------------------------------------------------------------------

statement_enum::iterator statement_enum::iterator::operator++( int )
{
   auto it = *this;
   next_statement();
   return it;
}

//-----------------------------------------------------------------------------

bool statement_enum::iterator::operator==( const iterator & it ) const
{
   return m_statement.from == it.m_statement.from &&
          m_statement.len == it.m_statement.len;
}

//-----------------------------------------------------------------------------

bool statement_enum::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}
