//-----------------------------------------------------------------------------

#ifndef DS_DB_FACTORY_H
#define DS_DB_FACTORY_H

//-----------------------------------------------------------------------------

#include <ds/factory.h>
#include <db/impl.h>
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
   typedef ds::factory< impl, std::string, const std::string & > db_factory_t;

   db_factory_t m_ds_factory;

public:

   typedef db_factory_t::Not_found_exception Not_found_exception;

   template< typename D > void register_db( void )
   {
      m_ds_factory.register_type< D >( D::TYPE );
   }

   impl * operator()( std::string connection ) const
   {
      std::string::size_type pos = connection.find( ":" );

      if ( pos == std::string::npos )
         throw std::runtime_error( "Poorly formed connection string: " + connection );

      return m_ds_factory( connection.substr( 0, pos ), connection.substr( pos + 1 ) );
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
