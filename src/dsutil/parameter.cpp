//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/parameter.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds::util::parameter
{

//-----------------------------------------------------------------------------

enumerator::enumerator( const std::string & sql, const char * delimiters ) :
m_sql( sql ),
m_delimiters( delimiters )
{
}

//-----------------------------------------------------------------------------

enumerator::iterator enumerator::begin( void ) const
{
   return { m_sql, m_delimiters };
}

//-----------------------------------------------------------------------------

enumerator::iterator enumerator::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

enumerator::iterator::iterator( const std::string & sql, const std::string & delimiters ) :
m_sql( sql ),
m_delimiters( delimiters )
{
   m_parameter.from = 0;
   m_parameter.len  = 0;

   next_parameter();
}

//-----------------------------------------------------------------------------

void enumerator::iterator::next_parameter( void )
{
   m_parameter.from += m_parameter.len;

   m_parameter.from = static_cast< uint32_t >( m_sql.find_first_of( m_delimiters, m_parameter.from ) );

   if ( m_parameter.from >= m_sql.length() )
   {
      m_parameter.from = std::numeric_limits< uint32_t >::max();
      m_parameter.len  = 0;
      return;
   }

   const char * to = m_sql.data() + m_parameter.from + 1;

   while ( *to && ( std::isalpha( *to ) || std::isdigit( *to ) || *to == '_' ) )
      to++;

   m_parameter.len = static_cast< uint32_t >( to - m_sql.data() ) - m_parameter.from;
}

//-----------------------------------------------------------------------------

const enumerator::iterator::parameter_t & enumerator::iterator::operator*( void ) const
{
   return m_parameter;
}

//-----------------------------------------------------------------------------

const enumerator::iterator::parameter_t * enumerator::iterator::operator->( void ) const
{
   return &m_parameter;
}

//-----------------------------------------------------------------------------

enumerator::iterator enumerator::iterator::operator++( void )
{
   next_parameter();
   return *this;
}

//-----------------------------------------------------------------------------

enumerator::iterator enumerator::iterator::operator++( int )
{
   auto it = *this;
   next_parameter();
   return it;
}

//-----------------------------------------------------------------------------

bool enumerator::iterator::operator==( const iterator & it ) const
{
   return m_parameter.from == it.m_parameter.from &&
          m_parameter.len == it.m_parameter.len;
}

//-----------------------------------------------------------------------------

bool enumerator::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}