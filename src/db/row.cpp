//-----------------------------------------------------------------------------

#include <db/row.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, int8_t & i )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, int16_t & i )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, int32_t & i )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, int64_t & i )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, uint8_t & u )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, uint16_t & u )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, uint32_t & u )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, uint64_t & u )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, double & d )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void row::impl::get_column( int index, std::string & s )
{
   throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

row::row( void )
{
   m_column = 0;
}

//-----------------------------------------------------------------------------

row::row( std::shared_ptr< impl > impl ) : row()
{
   m_impl = impl;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( int8_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( int16_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( int32_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( int64_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( uint8_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( uint16_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( uint32_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( uint64_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( double & d )
{
   m_impl->get_column( m_column++, d );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & row::operator >> ( std::string & s )
{
   m_impl->get_column( m_column++, s );
   return *this;
}

//-----------------------------------------------------------------------------

void row::endr( void )
{
   step();
}

//-----------------------------------------------------------------------------

bool row::step( void )
{
   if ( !m_impl->step() )
      return false;

   m_column = 0;

   return true;
}

//-----------------------------------------------------------------------------

bool row::operator == ( const row & r ) const
{
   return m_impl == r.m_impl;
}

//-----------------------------------------------------------------------------

bool row::operator != ( const row & r ) const
{
   return !( *this == r );
}

//-----------------------------------------------------------------------------

row::operator bool ( void ) const
{
   return m_impl != nullptr && *m_impl;
}

//-----------------------------------------------------------------------------

row::operator bool ( void )
{
   return *const_cast< const row * >( this );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
