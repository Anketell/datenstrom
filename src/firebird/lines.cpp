//-----------------------------------------------------------------------------

#include <firebird/lines.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

lines::lines( const std::string & lines ) :
m_lines( lines )
{
}

//-----------------------------------------------------------------------------

lines::iterator lines::begin( void ) const
{
   return { m_lines };
}

//-----------------------------------------------------------------------------

lines::iterator lines::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

lines::iterator::iterator( const std::string & lines ) :
m_lines( lines )
{
   m_line.to = m_lines.c_str();

   next_line();
}

//-----------------------------------------------------------------------------

void lines::iterator::next_line( void )
{
   if ( !m_line.to )
      return;

   m_line.from = m_line.to;

   while ( *m_line.from == '\n' || *m_line.from == '\r' )
      m_line.from++;

   if ( !*m_line.from )
   {
      m_line.from = m_line.to = nullptr;
      return;
   }

   m_line.to = m_line.from + 1;

   while ( *m_line.to && *m_line.to != '\n' && *m_line.to != '\r' )
      m_line.to++;
}

//-----------------------------------------------------------------------------

const lines::iterator::line_t & lines::iterator::operator*( void ) const
{
   return m_line;
}

//-----------------------------------------------------------------------------

const lines::iterator::line_t * lines::iterator::operator->( void ) const
{
   return &m_line;
}

//-----------------------------------------------------------------------------

lines::iterator lines::iterator::operator++( void )
{
   next_line();
   return *this;
}

//-----------------------------------------------------------------------------

lines::iterator lines::iterator::operator++( int )
{
   auto it = *this;
   next_line();
   return it;
}

//-----------------------------------------------------------------------------

bool lines::iterator::operator==( const iterator & it ) const
{
   return m_line.from == it.m_line.from && m_line.to == it.m_line.to;
}

//-----------------------------------------------------------------------------

bool lines::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}