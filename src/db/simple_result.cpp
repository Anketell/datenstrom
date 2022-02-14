//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/simple_result.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

simple_result::simple_result( uint64_t value )
{
   m_value = value;
   m_valid = true;
}

//-----------------------------------------------------------------------------

simple_result::~simple_result( void )
{
}

//-----------------------------------------------------------------------------

int simple_result::column_count( void ) const
{
   return 1;
}

//-----------------------------------------------------------------------------

int simple_result::rows_affected( void ) const
{
   return 1;
}

//-----------------------------------------------------------------------------

void simple_result::check_column( int index )
{
   if ( index >= column_count() )
      throw std::runtime_error( "Get simple result column: No column available" );
}

//-----------------------------------------------------------------------------

#define stringify( type ) #type

template< typename I > I simple_result::get_integer( int index )
{
   check_column( index );

   typedef std::numeric_limits< I > limits;

   if ( m_value > limits::max() )
      throw std::runtime_error( "Get simple result column: Integer data value too big" );

   return static_cast< I >( m_value );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, int8_t & i )
{
   i = get_integer< int8_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, int16_t & i )
{
   i = get_integer< int16_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, int32_t & i )
{
   i = get_integer< int32_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, int64_t & i )
{
   i = get_integer< int64_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, uint8_t & u )
{
   u = get_integer< uint8_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, uint16_t & u )
{
   u = get_integer< uint16_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, uint32_t & u )
{
   u = get_integer< uint32_t >( index );
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, uint64_t & u )
{
   check_column( index );
   u = m_value;
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, double & d )
{
   check_column( index );
   d = m_value;
}

//-----------------------------------------------------------------------------

void simple_result::get_column( int index, std::string & s )
{
   throw std::runtime_error( "Get simple result column: Not character type" );
}

//-----------------------------------------------------------------------------

bool simple_result::step( void )
{
   m_valid = false;
   return m_valid;
}

//-----------------------------------------------------------------------------

bool simple_result::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}
