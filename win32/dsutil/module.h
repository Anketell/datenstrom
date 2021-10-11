//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds::util
{

//-----------------------------------------------------------------------------

class module
{
   void * m_handle;

   void * find_symbol( const std::string & name ) const;

public:

   static constexpr char PATTERN[] = "*.dll";

   module( const std::string & path );

   template< typename F > F symbol( const std::string & name ) const
   {
      return reinterpret_cast< F >( find_symbol( name ) );
   }

   void close( void );

   operator bool ( void ) { return m_handle; }
};

//-----------------------------------------------------------------------------

}
