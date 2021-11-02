//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_DS_H
#define DS_DS_H

//-----------------------------------------------------------------------------

#include <string>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds
{

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

}

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream &, ds::istream::manip_t fn );
ds::ostream & operator << ( ds::ostream &, ds::ostream::manip_t fn );

//-----------------------------------------------------------------------------

#endif
