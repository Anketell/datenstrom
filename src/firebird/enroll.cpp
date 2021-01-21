//-----------------------------------------------------------------------------

#include <db/factory.h>
#include <firebird/connection.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

template<> impl * factory_helper< firebird::connection >::construct( const connect_params_t & params )
{
   auto server      = params[ "server" ];
   auto path        = params[ "path" ];
   auto port_str    = params[ "port" ];
   auto username    = params[ "username" ];
   auto password    = params[ "password" ];
   auto dialect_str = params[ "dialect" ];
   auto database    = params[ "database" ];
   auto ext         = params[ "ext" ];

   if ( !server.empty() )
   {
      if ( username.empty() )
         throw std::invalid_argument( "Connect string does not specify username" );

      if ( password.empty() )
         throw std::invalid_argument( "Connect string does not specify password" );
   }

   if ( path.empty() )
      throw std::invalid_argument( "Connect string specifies forbidden path" );

   if ( ext.empty() )
      ext = "fdb";

   int port = 3050;
   if ( !port_str.empty() )
      port = atoi( port_str.c_str() );

   int dialect = 3;
   if ( !dialect_str.empty() )
      dialect = atoi( dialect_str.c_str() );

   impl * db = new firebird::connection( server, path, username, password, ext, port, dialect );

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
   factory.register_impl< ds::firebird::connection >();
}

//-----------------------------------------------------------------------------
