//-----------------------------------------------------------------------------

#include <sqlite/constructor.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

template<> impl * constructor< sqlite::connection >( const connect_params_t & params )
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
