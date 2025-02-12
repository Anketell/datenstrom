//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <bin/bin.h>
#include <dsutil/endian.h>

//-----------------------------------------------------------------------------

namespace ds::bin
{

//-----------------------------------------------------------------------------

struct filter_t
{
   virtual uint16_t operator() ( uint16_t u ) = 0;
   virtual uint32_t operator() ( uint32_t u ) = 0;
   virtual uint64_t operator() ( uint64_t u ) = 0;
};

//-----------------------------------------------------------------------------

struct le_filter_t : filter_t
{
   virtual uint16_t operator() ( uint16_t u ) override { return endian::htol( u ); }
   virtual uint32_t operator() ( uint32_t u ) override { return endian::htol( u ); }
   virtual uint64_t operator() ( uint64_t u ) override { return endian::htol( u ); }
};

//-----------------------------------------------------------------------------

struct be_filter_t : filter_t
{
   virtual uint16_t operator() ( uint16_t u ) override { return endian::htob( u ); }
   virtual uint32_t operator() ( uint32_t u ) override { return endian::htob( u ); }
   virtual uint64_t operator() ( uint64_t u ) override { return endian::htob( u ); }
};

//-----------------------------------------------------------------------------

static le_filter_t le_filter;
static be_filter_t be_filter;
static le_filter_t null_filter;

//-----------------------------------------------------------------------------

istream::istream( std::streambuf * sb ) :
m_sb( sb ),
m_filter( &null_filter )
{
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int8_t & i )
{
   return *this >> *reinterpret_cast< uint8_t * >( &i );
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int16_t & i )
{
   return *this >> *reinterpret_cast< uint16_t * >( &i );
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int32_t & i )
{
   return *this >> *reinterpret_cast< uint32_t * >( &i );
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int64_t & i )
{
   return *this >> *reinterpret_cast< uint64_t * >( &i );
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint8_t & u )
{
   m_sb->sgetn( reinterpret_cast< char * >( &u ), sizeof( u ) );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint16_t & u )
{
   uint16_t tmp;
   m_sb->sgetn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   u = ( *m_filter )( tmp );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint32_t & u )
{
   uint32_t tmp;
   m_sb->sgetn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   u = ( *m_filter )( tmp );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint64_t & u )
{
   uint64_t tmp;
   m_sb->sgetn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   u = ( *m_filter )( tmp );
   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( double & d )
{
   m_sb->sgetn( reinterpret_cast< char * >( &d ), sizeof( d ) );
   return *this;
}

//-----------------------------------------------------------------------------

void istream::read( void * data, size_t size )
{
   m_sb->sgetn( reinterpret_cast< char * >( data ), size );
}

//-----------------------------------------------------------------------------

void istream::filter( filter_t * filter )
{
   m_filter = filter;
}

//-----------------------------------------------------------------------------

void istream::rdbuf( std::streambuf * sb )
{
   m_sb = sb;
}

//-----------------------------------------------------------------------------

std::streambuf * istream::rdbuf( void ) const
{
   return m_sb;
}

//-----------------------------------------------------------------------------

ostream::ostream( std::streambuf * sb ) :
m_sb( sb ),
m_filter( &null_filter )
{
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int8_t i )
{
   return *this << static_cast< uint8_t >( i );
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int16_t i )
{
   return *this << static_cast< uint16_t >( i );
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int32_t i )
{
   return *this << static_cast< uint32_t >( i );
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int64_t i )
{
   return *this << static_cast< uint64_t >( i );
}

//-----------------------------------------------------------------------------


ds::ostream & ostream::operator << ( uint8_t u )
{
   m_sb->sputn( reinterpret_cast< char * >( &u ), sizeof( u ) );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint16_t u )
{
   auto tmp = ( *m_filter )( u );
   m_sb->sputn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint32_t u )
{
   auto tmp = ( *m_filter )( u );
   m_sb->sputn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint64_t u )
{
   auto tmp = ( *m_filter )( u );
   m_sb->sputn( reinterpret_cast< char * >( &tmp ), sizeof( u ) );
   return *this;
}

//-----------------------------------------------------------------------------


ds::ostream & ostream::operator << ( double d )
{
   m_sb->sputn( reinterpret_cast< char * >( &d ), sizeof( d ) );
   return *this;
}

//-----------------------------------------------------------------------------

void ostream::write( const void * data, size_t size )
{
   m_sb->sputn( reinterpret_cast< const char * >( data ), size );
}

//-----------------------------------------------------------------------------

void ostream::filter( filter_t * filter )
{
   m_filter = filter;
}

//-----------------------------------------------------------------------------

void ostream::rdbuf( std::streambuf * sb )
{
   m_sb = sb;
}

//-----------------------------------------------------------------------------

std::streambuf * ostream::rdbuf( void ) const
{
   return m_sb;
}

//-----------------------------------------------------------------------------

streamwrap::streamwrap( void * buffer, std::streamsize length )
{
   setbuf( reinterpret_cast< char * >( buffer ), length );
}

//-----------------------------------------------------------------------------

std::streambuf * streamwrap::setbuf( char * s, std::streamsize n )
{
   setp( s, s + n );
   setg( s, s, s + n );

   return this;
}

//-----------------------------------------------------------------------------

int streamwrap::underflow( void )
{
   if ( !gremaining() )
      throw stream_underrun();

   return std::streambuf::underflow();
}

//-----------------------------------------------------------------------------

int streamwrap::overflow( int ch )
{
   if ( !premaining() )
      throw stream_overrun();

   auto res = std::streambuf::overflow( ch );

   setg( eback(), gptr(), pptr() );

   return res;
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::xsgetn( char * s, std::streamsize count )
{
   if ( gremaining() < count )
      throw stream_underrun();

   return std::streambuf::xsgetn( s, count );
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::xsputn( const char * s, std::streamsize count )
{
   if ( premaining() < count )
      throw stream_overrun();

   auto res = std::streambuf::xsputn( s, count );

   setg( eback(), gptr(), pptr() );

   return res;
}

//-----------------------------------------------------------------------------

std::streampos streamwrap::seekoff( std::streambuf::off_type off,
                                    std::ios_base::seekdir   way,
                                    std::ios_base::openmode  which )
{
   if ( which & std::ios::in )
   {
      std::streamoff new_off;
      switch ( way )
      {
         case std::ios::beg :
            new_off = off;
            break;

         case std::ios::cur :
            new_off = gptr() - eback() + off;
            break;

         case std::ios::end :
            new_off = egptr() - eback() + off;
            break;
      }

      if ( new_off < 0 )
         throw ds::stream_underrun();

      if ( new_off > egptr() - eback() )
         throw ds::stream_overrun();

      gbump( new_off - ( gptr() - eback() ) );
   }

   if ( which & std::ios::out )
   {
      std::streamoff new_off;
      switch ( way )
      {
         case std::ios::beg :
            new_off = off;
            break;

         case std::ios::cur :
            new_off = pptr() - pbase() + off;
            break;

         case std::ios::end :
            new_off = epptr() - pbase() + off;
            break;
      }

      if ( new_off < 0 )
         throw ds::stream_underrun();

      if ( new_off > epptr() - pbase() )
         throw ds::stream_overrun();

      pbump( new_off - ( pptr() - pbase() ) );
   }

   return -1;
}

//-----------------------------------------------------------------------------

void * streamwrap::buffer( void ) const
{
   return pbase();
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::size( void ) const
{
   return epptr() - pbase();
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::plength( void ) const
{
   return pptr() - pbase();
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::premaining( void ) const
{
   return epptr() - pptr();
}

//-----------------------------------------------------------------------------

void * streamwrap::ppos( void ) const
{
   return pptr();
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::glength( void ) const
{
   return gptr() - eback();
}

//-----------------------------------------------------------------------------

std::streamsize streamwrap::gremaining( void ) const
{
   return egptr() - gptr();
}

//-----------------------------------------------------------------------------

void * streamwrap::gpos( void ) const
{
   return gptr();
}

//-----------------------------------------------------------------------------

streambuf::streambuf( std::streamsize length ) :
streamwrap( new char[ length ], length )
{
}

//-----------------------------------------------------------------------------

streambuf::~streambuf( void )
{
   delete [] pbase();
}

//-----------------------------------------------------------------------------

lengthcalc::lengthcalc( void ) :
streamwrap( 0, 0x7fffffff )
{
}

//-----------------------------------------------------------------------------

int lengthcalc::underflow( void )
{
   return std::char_traits< char >::eof();
}

//-----------------------------------------------------------------------------

int lengthcalc::overflow( int ch )
{
   pbump( 1 );
   return ch;
}

//-----------------------------------------------------------------------------

std::streamsize lengthcalc::xsgetn( char * s, std::streamsize count )
{
   return count;
}

//-----------------------------------------------------------------------------

std::streamsize lengthcalc::xsputn( const char * s, std::streamsize count )
{
   pbump( count );
   return count;
}

//-----------------------------------------------------------------------------

ds::istream & big_endian( ds::istream & in )
{
   dynamic_cast< bin::istream & >( in ).filter( &bin::be_filter ) ;
   return in;
}

//-----------------------------------------------------------------------------

ds::ostream & big_endian( ds::ostream & out )
{
   dynamic_cast< bin::ostream & >( out ).filter( &bin::be_filter ) ;
   return out;
}

//-----------------------------------------------------------------------------

ds::istream & little_endian( ds::istream & in )
{
   dynamic_cast< bin::istream & >( in ).filter( &bin::le_filter ) ;
   return in;
}

//-----------------------------------------------------------------------------

ds::ostream & little_endian( ds::ostream & out )
{
   dynamic_cast< bin::ostream & >( out ).filter( &bin::le_filter ) ;
   return out;
}

//-----------------------------------------------------------------------------

}
