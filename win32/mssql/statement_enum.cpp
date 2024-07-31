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

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

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

statement_enum::iterator::token_t statement_enum::iterator::next_token( const char * from )
{
   token_t token = { skip_ws( from ), 0 };

   while ( *token.from && !std::isspace( *( token.from + token.len ) ) )
      token.len++;

   return token;
}

//-----------------------------------------------------------------------------

void statement_enum::iterator::next_statement( void )
{
   if ( !m_statement.from )
      return;

   m_statement.from = skip_ws( m_statement.from + m_statement.len );

   const char * to = m_statement.from;
   while ( *to )
   {
      if ( *to++ == ';' )
         break;
   }

   if ( !*m_statement.from )
   {
      m_statement.from = nullptr;
      m_statement.len  = 0;
      return;
   }

   m_statement.len = to - m_statement.from;
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

//-----------------------------------------------------------------------------

}