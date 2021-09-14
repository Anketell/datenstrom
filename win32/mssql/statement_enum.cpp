//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/statement_enum.h>
#include <util/string.h>

#include <functional>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

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

static const char * skipws( const char * c )
{
   while ( *c && std::isspace( *c ) )
      c++;

   return c;
}

//-----------------------------------------------------------------------------

statement_enum::iterator::token_t statement_enum::iterator::next_token( const char * from )
{
   token_t token = { skipws( from ), 0 };

   while ( *token.from && !std::isspace( *( token.from + token.len ) ) )
      token.len++;

   return token;
}

//-----------------------------------------------------------------------------

void statement_enum::iterator::next_statement( void )
{
   static const std::string create( "create" );
   static const std::string function( "function" );

   static const std::function< bool( int, int )> CmpNoCase = []( int c1, int c2 )
   {
      return std::tolower( c1 ) == std::tolower( c2 );
   };

   if ( !m_statement.from )
      return;

   m_statement.from = skipws( m_statement.from + m_statement.len );

   const char * from = m_statement.from;
   while ( *from )
   {
      token_t token = next_token( from );

      if ( std::equal( token.from, token.from + token.len, create.begin(), create.end(), CmpNoCase ) )
      {
         token_t object = next_token( token.from + token.len );
         if ( std::equal( object.from, object.from + object.len, function.begin(), function.end(), CmpNoCase ) )
         {
            if ( m_statement.from != token.from )
            {
               from = token.from;
               break;
            }
         }
      }
      from = token.from + token.len;
   }

   if ( !*m_statement.from )
   {
      m_statement.from = nullptr;
      m_statement.len  = 0;
      return;
   }

   m_statement.len = from - m_statement.from;
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