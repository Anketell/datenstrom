//-----------------------------------------------------------------------------

#include <db/statement.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, int8_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, int16_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, int32_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, int64_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, uint8_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, uint16_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, uint32_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, uint64_t )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, double )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, const char * )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void statement::impl::set_parameter( int index, const std::string & )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

statement::statement( std::shared_ptr< impl > impl )
{
   m_impl      = impl;
   m_parameter = 1;
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

row statement::result( void )
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

statement::iterator::iterator( row row )
{
   m_row = row;
}

//-----------------------------------------------------------------------------

row statement::iterator::operator*( void )
{
   if ( m_row == row() )
      throw std::runtime_error( "No row available" );

   return m_row;
}

//-----------------------------------------------------------------------------

statement::iterator & statement::iterator::operator++( void )
{
   if ( !m_row.step() )
   {
      m_row = row();
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
   return m_row == it.m_row;
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
