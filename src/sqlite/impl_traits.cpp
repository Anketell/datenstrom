//-----------------------------------------------------------------------------

#include <sqlite/impl_traits.h>
#include <db/connect_params.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

constexpr char impl_traits< sqlite::connection >::TYPE[];

//-----------------------------------------------------------------------------

impl * impl_traits< sqlite::connection >::construct( const db::connect_params_t & params )
{
   auto location = params[ "location" ];
   auto database = params[ "database" ];

   if ( location.empty() )
      throw std::invalid_argument( "Connect string does not specify location" );

   impl * db = new sqlite::connection( "/" + location );

   if ( !database.empty() )
      db->use( database );

   return db;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
