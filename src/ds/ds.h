//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <string>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

class istream;

//-----------------------------------------------------------------------------

template< typename T > struct has_global_get_from_operator_t
{
   template< typename V > static auto test( int ) ->
   decltype( operator>>( std::declval< ds::istream & >(), std::declval< V & >() ) );

   template< typename... > static auto test( ... ) -> std::false_type;

   using type = typename std::is_same< ds::istream &, decltype( test< T >( 0 ) ) >::type;
};

//-----------------------------------------------------------------------------

template< typename T, typename O > using member_get_from_operator_t =
decltype( std::declval< O & >().operator>>( std::declval< T & >() ) );

//-----------------------------------------------------------------------------

template< typename T >
constexpr bool has_global_get_from = typename has_global_get_from_operator_t< T >::type();

template< typename T, typename O, typename = void >
constexpr bool has_member_get_from = false;

template< typename T, typename O >
constexpr bool has_member_get_from< T, O, std::void_t< member_get_from_operator_t< T, O >() > > = true;

template< typename T >
constexpr bool has_istream_get_from = has_member_get_from< T, ds::istream >;

template< typename T >
constexpr bool has_get_from = has_global_get_from< T > || has_istream_get_from< T >;

//-----------------------------------------------------------------------------

class Not_implemented : public std::runtime_error
{
public:

   Not_implemented( void );
};

//-----------------------------------------------------------------------------

class istream
{
public:

   typedef istream & ( *manip_t )( istream & );

   virtual ~istream( void );

   virtual istream & operator >> ( int8_t & );
   virtual istream & operator >> ( int16_t & );
   virtual istream & operator >> ( int32_t & );
   virtual istream & operator >> ( int64_t & );

   virtual istream & operator >> ( uint8_t & );
   virtual istream & operator >> ( uint16_t & );
   virtual istream & operator >> ( uint32_t & );
   virtual istream & operator >> ( uint64_t & );

   virtual istream & operator >> ( double & );

   virtual istream & operator >> ( std::string & );

   virtual void read( void *, size_t );

   virtual bool get_null( void );

   virtual void endr( void );

   virtual bool eof( void ) const;

   virtual const std::string & name( void );

   template< typename T, std::enable_if_t< has_get_from< T >, bool > = true >
   operator T ( void )
   {
      T t;
      *this >> t;
      return t;
   }
};

//-----------------------------------------------------------------------------

class ostream
{
public:

   typedef ostream & ( *manip_t )( ostream & );

   virtual ~ostream( void );

   virtual ostream & operator << ( int8_t );
   virtual ostream & operator << ( int16_t );
   virtual ostream & operator << ( int32_t );
   virtual ostream & operator << ( int64_t );

   virtual ostream & operator << ( uint8_t );
   virtual ostream & operator << ( uint16_t );
   virtual ostream & operator << ( uint32_t );
   virtual ostream & operator << ( uint64_t );

   virtual ostream & operator << ( double );

   virtual ostream & operator << ( const char * );
   virtual ostream & operator << ( const std::string & );

   virtual void write( const void *, size_t );

   virtual void put_null( void );

   virtual void endr( void );

   virtual const std::string & name( void );

   template< typename T > ostream & operator()( const T & t )
   {
      *this << t;
      return *this;
   }

   template< typename T, typename ... Rest >
   ostream & operator()( const T & t, const Rest & ... rest )
   {
      *this << t;
      return operator()( rest... );
   }
};

//-----------------------------------------------------------------------------

istream & endr( istream & in );
ostream & endr( ostream & out );

//-----------------------------------------------------------------------------

class exception : public std::runtime_error
{
protected:

   exception( const std::string & what );
   exception( const char        * what );
};

//-----------------------------------------------------------------------------

class stream_underrun : public exception
{
public:

   stream_underrun( void );
};

//-----------------------------------------------------------------------------

class stream_overrun : public exception
{
public:

   stream_overrun( void );
};

//-----------------------------------------------------------------------------

class null_value : public exception
{
public:

   null_value( void );
};

//-----------------------------------------------------------------------------

istream & operator >> ( istream &, istream::manip_t fn );
ostream & operator << ( ostream &, ostream::manip_t fn );

//-----------------------------------------------------------------------------

}

