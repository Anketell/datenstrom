//-----------------------------------------------------------------------------

#include <db/statement.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

statement::statement( std::shared_ptr< impl > impl )
{
   m_impl      = impl;
   m_parameter = 1;
}

//-----------------------------------------------------------------------------

int statement::parameter_count( void ) const
{
   return m_impl->parameter_count();
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int8_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int16_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int32_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int64_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint8_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint16_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint32_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint64_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( double d )
{
   m_impl->set_parameter( m_parameter++, d );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( const char * s )
{
   m_impl->set_parameter( m_parameter++, s );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( const std::string & s )
{
   m_impl->set_parameter( m_parameter++, s );
   return *this;
}

//-----------------------------------------------------------------------------

void statement::endr( void )
{
   execute();
}

//-----------------------------------------------------------------------------

void statement::reset( void )
{
   m_impl->reset();
}

//-----------------------------------------------------------------------------

uint32_t statement::execute( void )
{
   if ( m_parameter > 0 && m_parameter != m_impl->parameter_count() + 1 )
      throw std::runtime_error( "Wrong number of parameters" );

   m_parameter = 1;
   return m_impl->execute();
}

//-----------------------------------------------------------------------------

result statement::result( void )
{
   if ( m_parameter > 0 && m_parameter != m_impl->parameter_count() + 1 )
      throw std::runtime_error( "Wrong number of parameters" );

   m_parameter = 1;
   return m_impl->result();
}

//-----------------------------------------------------------------------------

statement::iterator statement::begin( void )
{
   return iterator( result() );
}

//-----------------------------------------------------------------------------

statement::iterator statement::end( void )
{
   return iterator();
}

//-----------------------------------------------------------------------------

statement::iterator::iterator( void )
{
}

//-----------------------------------------------------------------------------

statement::iterator::iterator( db::result result )
{
   m_result = result;
}

//-----------------------------------------------------------------------------

result statement::iterator::operator*( void )
{
   if ( !m_result )
      throw std::runtime_error( "No result available" );

   return m_result;
}

//-----------------------------------------------------------------------------

statement::iterator & statement::iterator::operator++( void )
{
   if ( !m_result.step() )
   {
      m_result = db::result();
   }

   return *this;
}

//-----------------------------------------------------------------------------

statement::iterator & statement::iterator::operator++( int )
{
   return ++*this;
}

//-----------------------------------------------------------------------------

bool statement::iterator::operator==( const iterator &it ) const
{
   return m_result == it.m_result;
}

//-----------------------------------------------------------------------------

bool statement::iterator::operator!=( const iterator &it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
