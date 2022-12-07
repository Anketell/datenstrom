//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/statement_enum.h>
#include <dsutil/string.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
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

bool statement_enum::iterator::is_term_statement( void )
{
   static constexpr char set[]    = "set ";
   static constexpr char term[]   = "term ";
   static constexpr int  set_len  = sizeof( set )  - 1;
   static constexpr int  term_len = sizeof( term ) - 1;

   if ( ds::string::cmpignorecase( set, m_statement.from, set_len ) != 0 )
      return false;

   const char * p = m_statement.from + set_len;

   while ( std::isspace( *p ) )
      p++;

   if ( ds::string::cmpignorecase( term, p, term_len ) != 0 )
      return false;

   p += term_len;

   while ( std::isspace( *p ) )
      p++;

   uint32_t eos = m_statement.len - ( p - m_statement.from );
   uint32_t len = 0;
   while ( len < eos && !std::isspace( *( p + len ) ) )
      len++;

   if ( !len )
      return false;

   if ( *( m_statement.from + m_statement.len ) )
      m_statement.len += m_separator.length();

   m_separator.assign( p, len );

   return true;
}

//-----------------------------------------------------------------------------

void statement_enum::iterator::next_statement( void )
{
   if ( !m_statement.from )
      return;

   do
   {
      m_statement.from += m_statement.len;

      for ( ;; )
      {
         if ( std::isspace( *m_statement.from ) )
         {
            m_statement.from++;
            continue;
         }

         if ( strncmp( m_statement.from, m_separator.c_str(), m_separator.length() ) == 0 )
         {
            m_statement.from += m_separator.length();
            continue;
         }

         break;
      }

      if ( !*m_statement.from )
      {
         m_statement.from = nullptr;
         m_statement.len  = 0;
         return;
      }

      const char *to = m_statement.from + 1;

      while ( *to && strncmp( to, m_separator.c_str(), m_separator.length() ) != 0 )
         to++;

      m_statement.len = to - m_statement.from;
   }
   while ( is_term_statement() );
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