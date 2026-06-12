//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <ds/ds.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

Not_implemented::Not_implemented( void ) :
std::runtime_error( "Not implemented" )
{
}

//-----------------------------------------------------------------------------

istream::~istream( void  )
{
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( int8_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( int16_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( int32_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( int64_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( uint8_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( uint16_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( uint32_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( uint64_t & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( double & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & istream::operator >> ( std::string & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

void istream::read( void *, size_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

bool istream::get_null( void )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

void istream::endr( void )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream::~ostream( void  )
{
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( int8_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( int16_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( int32_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( int64_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( uint8_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( uint16_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( uint32_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( uint64_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( double )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( const char * )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

ostream & ostream::operator << ( const std::string & )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

void ostream::write( const void *, size_t )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

void ostream::put_null( void )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

void ostream::endr( void )
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

bool istream::eof( void ) const
{
   throw Not_implemented();
}

//-----------------------------------------------------------------------------

istream & endr( istream & in )
{
   in.endr();
   return in;
}

//-----------------------------------------------------------------------------

ostream & endr( ostream & out )
{
   out.endr();
   return out;
}

//-----------------------------------------------------------------------------

exception::exception( const std::string & what ) :
std::runtime_error( what )
{
}

//-----------------------------------------------------------------------------

exception::exception( const char * what ) :
std::runtime_error( what )
{
}

//-----------------------------------------------------------------------------

stream_underrun::stream_underrun( void ) :
exception( "Stream underrun" )
{
}

//-----------------------------------------------------------------------------

stream_overrun::stream_overrun( void ) :
exception( "Stream overrun" )
{
}

//-----------------------------------------------------------------------------

null_value::null_value( void ) :
exception( "Null value" )
{
}

//-----------------------------------------------------------------------------

istream & operator >> ( istream & in, istream::manip_t manip )
{
   return manip( in );
}

//-----------------------------------------------------------------------------

ostream & operator << ( ostream & out, ostream::manip_t manip )
{
   return manip( out );
}

//-----------------------------------------------------------------------------

}

