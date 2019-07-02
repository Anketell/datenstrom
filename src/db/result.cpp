//-----------------------------------------------------------------------------

#include <db/result.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

result::result( void )
{
   m_column = 0;
}

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< impl > impl ) : result()
{
   m_impl = impl;
}

//-----------------------------------------------------------------------------

int result::column_count( void ) const
{
   return m_impl->column_count();
}

//-----------------------------------------------------------------------------

int result::rows_affected( void ) const
{
   return m_impl->rows_affected();
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( int8_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( int16_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( int32_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( int64_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( uint8_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( uint16_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( uint32_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( uint64_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( double & d )
{
   m_impl->get_column( m_column++, d );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & result::operator >> ( std::string & s )
{
   m_impl->get_column( m_column++, s );
   return *this;
}

//-----------------------------------------------------------------------------

void result::endr( void )
{
   step();
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   m_column = 0;

   if ( !m_impl->step() )
   {
      m_impl = nullptr;
      return false;
   }

   return true;
}

//-----------------------------------------------------------------------------

result::operator bool ( void ) const
{
   return m_impl != nullptr && *m_impl;
}

//-----------------------------------------------------------------------------

result::operator bool ( void )
{
   return *const_cast< const result * >( this );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
