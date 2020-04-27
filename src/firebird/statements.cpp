//-----------------------------------------------------------------------------

#include <firebird/statements.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

statements::statements( const std::string & statements ) :
m_statements( statements )
{
}

//-----------------------------------------------------------------------------

statements::iterator statements::begin( void ) const
{
   return { m_statements };
}

//-----------------------------------------------------------------------------

statements::iterator statements::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

statements::iterator::iterator( const std::string & statements ) :
m_statements( statements )
{
   m_statement.from = m_statements.c_str();
   m_statement.len  = 0;

   next_line();
}

//-----------------------------------------------------------------------------

void statements::iterator::next_line( void )
{
   if ( !m_statement.from )
      return;

   m_statement.from += m_statement.len;

   while ( *m_statement.from == ';' || std::isspace( *m_statement.from ) )
      m_statement.from++;

   if ( !*m_statement.from )
   {
      m_statement.from = nullptr;
      m_statement.len  = 0;
      return;
   }

   const char *to = m_statement.from + 1;

   while ( *to && *to != ';' )
      to++;

   m_statement.len = to - m_statement.from;
}

//-----------------------------------------------------------------------------

const statements::iterator::statement_t & statements::iterator::operator*( void ) const
{
   return m_statement;
}

//-----------------------------------------------------------------------------

const statements::iterator::statement_t * statements::iterator::operator->( void ) const
{
   return &m_statement;
}

//-----------------------------------------------------------------------------

statements::iterator statements::iterator::operator++( void )
{
   next_line();
   return *this;
}

//-----------------------------------------------------------------------------

statements::iterator statements::iterator::operator++( int )
{
   auto it = *this;
   next_line();
   return it;
}

//-----------------------------------------------------------------------------

bool statements::iterator::operator==( const iterator & it ) const
{
   return m_statement.from == it.m_statement.from &&
          m_statement.len == it.m_statement.len;
}

//-----------------------------------------------------------------------------

bool statements::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}