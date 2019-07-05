//-----------------------------------------------------------------------------

#include <db/factory.h>
#include <sqlite/connection.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class connect_params_t;

//-----------------------------------------------------------------------------

template<> struct factory_helper< sqlite::connection >
{
   static constexpr char TYPE[] = "sqlite";
   static impl * construct( const db::connect_params_t & params );
};

//-----------------------------------------------------------------------------

constexpr char factory_helper< sqlite::connection >::TYPE[];

//-----------------------------------------------------------------------------

impl * factory_helper< sqlite::connection >::construct( const db::connect_params_t & params )
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

//-----------------------------------------------------------------------------

extern "C" void enroll( ds::db::factory & factory )
{
   factory.register_impl< ds::sqlite::connection >();
}

//-----------------------------------------------------------------------------
