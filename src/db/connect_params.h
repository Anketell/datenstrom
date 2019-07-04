//-----------------------------------------------------------------------------

#ifndef DS_DB_CONNECT_PARAMS_H
#define DS_DB_CONNECT_PARAMS_H

//-----------------------------------------------------------------------------

#include <map>
#include <string>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class impl;

//-----------------------------------------------------------------------------

class connect_params_t : public std::map< std::string, std::string >
{
public:

   std::string & operator[]( const std::string & key )
   {
      return std::map< std::string, std::string >::operator[]( key );
   }

   std::string operator[]( const std::string & key ) const
   {
      auto it = find( key );
      if ( it == end() )
         return "";

      return it->second;
   }
};

//-----------------------------------------------------------------------------

template< typename D > impl * construct( const connect_params_t & params )
{
   throw std::runtime_error( params[ "type" ] + " constructor not implemented" );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
