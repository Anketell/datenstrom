//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/url_encode.h>
#include <sstream>
#include <iomanip>
#include <string.h>

//-----------------------------------------------------------------------------

namespace ds::url
{

//-----------------------------------------------------------------------------

std::string unescape( const std::string & s )
{
   std::string res;

   const char * p = s.c_str();

   enum { Text, Hex } state = Text;

   int     width;
   wchar_t value;

   while ( *p )
   {
      switch ( state )
      {
         case Text :
            if ( *p == '%' )
            {
               state = Hex;
               width = 2;
               value = 0;
            }
            else
               res += *p;
            p++;
            break;

         case Hex :
         {
            char c = tolower( *p );
            if ( isdigit( c ) )
               value = value * 16 + c - '0';
            else
            {
               if ( c >= 'a' && c <= 'f' )
                  value = value * 16 + c - 'a' + 10;
                else
               {
                  res  += value;
                  state = Text;
                  break;
               }
            }

            p++;

            if ( --width == 0 )
            {
               res  += value;
               state = Text;
            }
            break;
         }
      }
   }

   return res;
}

//-----------------------------------------------------------------------------

std::string escape( const std::string & s )
{
   static const char no_esc[] = "-_.~";

   std::stringstream str;

   for ( auto c : s )
   {
      if ( isdigit( c ) || isalpha( c ) || strchr( no_esc, c ) != NULL )
         str << ( char )c;
      else
      {
         str << "%" << std::setw( 2 )
               << std::setfill( '0' )
               << std::hex << ( int )c;
      }
   }

   return str.str();
}

//-----------------------------------------------------------------------------

}
