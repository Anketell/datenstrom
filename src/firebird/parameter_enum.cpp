//-----------------------------------------------------------------------------

#include <firebird/parameter_enum.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

parameter_enum::parameter_enum( const std::string & sql ) :
m_sql( sql )
{
}

//-----------------------------------------------------------------------------

parameter_enum::iterator parameter_enum::begin( void ) const
{
   return { m_sql };
}

//-----------------------------------------------------------------------------

parameter_enum::iterator parameter_enum::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

parameter_enum::iterator::iterator( const std::string & sql ) :
m_sql( sql )
{
   m_parameter.from = 0;
   m_parameter.len  = 0;

   next_parameter();
}

//-----------------------------------------------------------------------------

void parameter_enum::iterator::next_parameter( void )
{
   m_parameter.from += m_parameter.len;

   while ( m_parameter.from < m_sql.length() && m_sql[ m_parameter.from ] != ':' )
      m_parameter.from++;

   if ( m_parameter.from >= m_sql.length() )
   {
      m_parameter.from = std::numeric_limits< uint32_t >::max();
      m_parameter.len  = 0;
      return;
   }

   const char * to = m_sql.data() + m_parameter.from + 1;

   while ( *to && ( std::isalpha( *to ) || std::isdigit( *to ) || *to == '_' ) )
      to++;

   m_parameter.len = to - m_sql.data() - m_parameter.from;
}

//-----------------------------------------------------------------------------

const parameter_enum::iterator::parameter_t & parameter_enum::iterator::operator*( void ) const
{
   return m_parameter;
}

//-----------------------------------------------------------------------------

const parameter_enum::iterator::parameter_t * parameter_enum::iterator::operator->( void ) const
{
   return &m_parameter;
}

//-----------------------------------------------------------------------------

parameter_enum::iterator parameter_enum::iterator::operator++( void )
{
   next_parameter();
   return *this;
}

//-----------------------------------------------------------------------------

parameter_enum::iterator parameter_enum::iterator::operator++( int )
{
   auto it = *this;
   next_parameter();
   return it;
}

//-----------------------------------------------------------------------------

bool parameter_enum::iterator::operator==( const iterator & it ) const
{
   return m_parameter.from == it.m_parameter.from &&
          m_parameter.len == it.m_parameter.len;
}

//-----------------------------------------------------------------------------

bool parameter_enum::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}