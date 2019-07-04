//-----------------------------------------------------------------------------

#ifndef DS_DB_FACTORY_H
#define DS_DB_FACTORY_H

//-----------------------------------------------------------------------------

#include <ds/factory.h>
#include <db/impl_traits.h>
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

      Not_found_exception( const std::string & type ) :
      std::invalid_argument( "Unknown database type: " + type )
      {
      }
   };

   template< typename D > void register_impl( void )
   {
      m_constructor_map[ impl_traits< D >::TYPE ] = impl_traits< D >::construct;
   }

   impl * operator()( const std::string & connect_string ) const
   {
      connect_params_t params = parse_connect_string( connect_string );
      auto it = m_constructor_map.find( params[ "type" ] );
      if ( it == m_constructor_map.end() )
         throw Not_found_exception( params[ "type" ] );

      return it->second( params );
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
