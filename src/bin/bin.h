//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_BIN_H
#define DS_BIN_H

//-----------------------------------------------------------------------------

#include <ds/ds.h>
#include <streambuf>
#include <ios>

//-----------------------------------------------------------------------------

namespace ds::bin
{

//-----------------------------------------------------------------------------

struct filter_t;

//-----------------------------------------------------------------------------

class istream : public ds::istream
{
   filter_t * m_filter;

protected:

   std::streambuf * m_sb;

public:

   istream( std::streambuf * sb = nullptr );

   virtual ds::istream & operator >> ( int8_t & ) override;
   virtual ds::istream & operator >> ( int16_t & ) override;
   virtual ds::istream & operator >> ( int32_t & ) override;
   virtual ds::istream & operator >> ( int64_t & ) override;

   virtual ds::istream & operator >> ( uint8_t & ) override;
   virtual ds::istream & operator >> ( uint16_t & ) override;
   virtual ds::istream & operator >> ( uint32_t & ) override;
   virtual ds::istream & operator >> ( uint64_t & ) override;

   virtual ds::istream & operator >> ( double & ) override;

   virtual void read( void *, size_t ) override;

   void filter( filter_t * filter );
   void rdbuf( std::streambuf * sb );
   std::streambuf * rdbuf( void ) const;
};

//-----------------------------------------------------------------------------

class ostream : public ds::ostream
{
   filter_t * m_filter;

protected:

   std::streambuf * m_sb;

public:

   ostream( std::streambuf * sb = nullptr );

   virtual ds::ostream & operator << ( int8_t ) override;
   virtual ds::ostream & operator << ( int16_t ) override;
   virtual ds::ostream & operator << ( int32_t ) override;
   virtual ds::ostream & operator << ( int64_t ) override;

   virtual ds::ostream & operator << ( uint8_t ) override;
   virtual ds::ostream & operator << ( uint16_t ) override;
   virtual ds::ostream & operator << ( uint32_t ) override;
   virtual ds::ostream & operator << ( uint64_t ) override;

   virtual ds::ostream & operator << ( double ) override;

   virtual void write( const void *, size_t ) override;

   void filter( filter_t * filter );
   void rdbuf( std::streambuf * sb );
   std::streambuf * rdbuf( void ) const;
};

//-----------------------------------------------------------------------------

class streamwrap : public std::streambuf
{
protected:

   virtual std::streambuf * setbuf( char * s, std::streamsize n ) override;

   virtual int underflow( void ) override;
   virtual int overflow( int ch ) override;

   virtual std::streamsize xsgetn( char * s, std::streamsize count ) override;
   virtual std::streamsize xsputn( const char * s, std::streamsize count ) override;

   virtual std::streampos seekoff( std::streambuf::off_type off,
                                   std::ios_base::seekdir way,
                                   std::ios_base::openmode which = std::ios::in | std::ios::out ) override;

public:

   streamwrap( void * buffer, std::streamsize length );

   void * buffer( void ) const;
   std::streamsize size( void ) const;

   std::streamsize plength( void ) const;
   std::streamsize premaining( void ) const;
   void          * ppos( void ) const;

   std::streamsize glength( void ) const;
   std::streamsize gremaining( void ) const;
   void          * gpos( void ) const;
};

//-----------------------------------------------------------------------------

class streambuf : public streamwrap
{
public:

   streambuf( std::streamsize length );
   virtual ~streambuf( void );
};

//-----------------------------------------------------------------------------

class lengthcalc : public streamwrap
{
protected:

   virtual int underflow( void ) override;
   virtual int overflow( int ch ) override;

   virtual std::streamsize xsgetn( char * s, std::streamsize count ) override;
   virtual std::streamsize xsputn( const char * s, std::streamsize count ) override;

public:

   lengthcalc( void );
};

//-----------------------------------------------------------------------------

template< typename S, typename T > std::streamsize length( const T & t )
{
   lengthcalc lc;
   S out( &lc );
   dynamic_cast< ds::ostream & >( out ) << t;
   return lc.plength();
}

//-----------------------------------------------------------------------------

template< typename T > std::streamsize length( const T & t )
{
   return length< ostream, T >( t );
}

//-----------------------------------------------------------------------------

class buffer_underrun : public std::runtime_error
{
public:

   buffer_underrun( void );
};

//-----------------------------------------------------------------------------

class buffer_overrun : public std::runtime_error
{
public:

   buffer_overrun( void );
};

//-----------------------------------------------------------------------------

ds::istream & big_endian( ds::istream & in );
ds::ostream & big_endian( ds::ostream & out );
ds::istream & little_endian( ds::istream & in );
ds::ostream & little_endian( ds::ostream & out );

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
