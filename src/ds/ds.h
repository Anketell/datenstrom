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

   virtual void endr( void );

   virtual bool eof( void ) const;

   template< typename T, std::enable_if_t< has_get_from< T >, bool > = false >
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

   virtual void endr( void );
};

//-----------------------------------------------------------------------------

istream & endr( istream & in );
ostream & endr( ostream & out );

//-----------------------------------------------------------------------------

class stream_underrun : public std::runtime_error
{
public:

   stream_underrun( void );
};

//-----------------------------------------------------------------------------

class stream_overrun : public std::runtime_error
{
public:

   stream_overrun( void );
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream &, ds::istream::manip_t fn );
ds::ostream & operator << ( ds::ostream &, ds::ostream::manip_t fn );

//-----------------------------------------------------------------------------
