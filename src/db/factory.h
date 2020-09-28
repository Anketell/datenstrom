//-----------------------------------------------------------------------------

#ifndef DS_DB_FACTORY_H
#define DS_DB_FACTORY_H

//-----------------------------------------------------------------------------

#include <db/factory_helper.h>
#include <stdexcept>

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
      m_constructor_map[ factory_helper< D >::TYPE ] = factory_helper< D >::construct;
   }

   impl * operator()( const connect_params_t & params ) const
   {
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
