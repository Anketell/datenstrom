//-----------------------------------------------------------------------------

#ifndef DS_DB_FACTORY_H
#define DS_DB_FACTORY_H

//-----------------------------------------------------------------------------

#include <ds/factory.h>
#include <db/impl.h>
#include <db/connect_string.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class factory
{
   typedef impl * ( * constructor_t )( const connect_params_t & );
   typedef std::map< std::string, constructor_t > constructor_map_t;

   constructor_map_t m_constructor_map;

public:

   class Not_found_exception : public std::invalid_argument
   {
   public :

      Not_found_exception( void ) :
      std::invalid_argument( "Unknown database type" )
      {
      }
   };

   template< typename D > void register_impl( void )
   {
      m_constructor_map[ D::TYPE ] = construct< D >;
   }

   impl * operator()( const std::string & connect_string ) const
   {
      connect_params_t params = parse_connect_string( connect_string );
      auto it = m_constructor_map.find( params[ "type" ] );
      if ( it == m_constructor_map.end() )
         throw Not_found_exception();

      return it->second( params );
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
