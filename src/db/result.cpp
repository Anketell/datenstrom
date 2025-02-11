//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/result.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

rowset::rowset( void )
{
   m_column = 0;
}

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< impl > impl ) : rowset()
{
   m_impl = impl;
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return m_impl->column_count();
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   return m_impl->rows_affected();
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( int8_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( int16_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( int32_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( int64_t & i )
{
   m_impl->get_column( m_column++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( uint8_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( uint16_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( uint32_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( uint64_t & u )
{
   m_impl->get_column( m_column++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( double & d )
{
   m_impl->get_column( m_column++, d );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & rowset::operator >> ( std::string & s )
{
   m_impl->get_column( m_column++, s );
   return *this;
}

//-----------------------------------------------------------------------------

void rowset::endr( void )
{
   step();
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
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

bool rowset::eof( void ) const
{
   return m_impl == nullptr || m_impl->eof();
}

//-----------------------------------------------------------------------------

bool rowset::operator==( const rowset & r ) const
{
   return this->eof() && r.eof() || m_impl == r.m_impl;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
