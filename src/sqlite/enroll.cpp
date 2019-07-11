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

template<> impl * factory_helper< sqlite::connection >::construct( const connect_params_t & params )
{
   auto server   = params[ "server" ];
   auto path     = params[ "path" ];
   auto database = params[ "database" ];

   if ( !server.empty() )
      throw std::invalid_argument( "Connect string specifies forbiden server" );

   if ( path.empty() )
      throw std::invalid_argument( "Connect string does not specify path" );

   impl * db = new sqlite::connection( path );

   if ( !database.empty() )
   {
      try
      {
         db->use( database );
      }
      catch ( ... )
      {
         delete db;
         throw;
      }
   }

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
