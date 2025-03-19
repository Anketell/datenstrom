//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <csv/csv.h>
#include <ostream>
#include <iomanip>

//-----------------------------------------------------------------------------

namespace ds::csv
{

//-----------------------------------------------------------------------------

static void write( std::ostream & out, const char * s )
{
   out << '\"';

   while ( *s )
   {
      switch ( *s )
      {
         case '"':
            out << *s;
            break;

         case '\r':
         case '\n':
         case '\\':
            out << '\\';
      }
      out << *s++;
   }

   out << '\"';
}

//-----------------------------------------------------------------------------

static void prolog( std::ostream & out )
{
   out << "";
}

//-----------------------------------------------------------------------------

static void epilog( std::ostream & out )
{
   out << std::setfill( ',' ) << std::setw( 1 );
}

//-----------------------------------------------------------------------------

ostream::ostream( std::ostream & out ) :
m_out( out )
{
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int8_t i )
{
   prolog( m_out );
   m_out << ( int )i;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int16_t i )
{
   prolog( m_out );
   m_out << i;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int32_t i )
{
   prolog( m_out );
   m_out << i;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( int64_t i )
{
   prolog( m_out );
   m_out << i;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint8_t u )
{
   prolog( m_out );
   m_out << ( unsigned )u;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint16_t u )
{
   prolog( m_out );
   m_out << u;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint32_t u )
{
   prolog( m_out );
   m_out << u;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( uint64_t u )
{
   prolog( m_out );
   m_out << u;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( const char * s )
{
   prolog( m_out );
   csv::write( m_out, s );
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( const std::string &s )
{
   prolog( m_out );
   csv::write( m_out, s.c_str() );
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & ostream::operator << ( double d )
{
   prolog( m_out );
   m_out << d;
   epilog( m_out  );

   return *this;
}

//-----------------------------------------------------------------------------

void ostream::endr( void )
{
   m_out << std::endl << std::setw( 0 );
}

//-----------------------------------------------------------------------------

}
