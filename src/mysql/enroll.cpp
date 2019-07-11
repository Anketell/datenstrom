//-----------------------------------------------------------------------------

#include <db/factory.h>
#include <mysql/connection.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

template<> impl * factory_helper< mysql::connection >::construct( const connect_params_t & params )
{
   auto server   = params[ "server" ];
   auto path     = params[ "path" ];
   auto port_str = params[ "port" ];
   auto username = params[ "username" ];
   auto password = params[ "password" ];
   auto database = params[ "database" ];

   if ( server.empty() )
      throw std::invalid_argument( "Connect string does not specify server" );

   if ( !path.empty() )
      throw std::invalid_argument( "Connect string specifies forbidden path" );

   if ( username.empty() )
      throw std::invalid_argument( "Connect string does not specify username" );

   int port = 3306;
   if ( !port_str.empty() )
      port = atoi( port_str.c_str() );

   return new mysql::connection( database, server, username, password, port );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

extern "C" void enroll( ds::db::factory & factory )
{
   factory.register_impl< ds::mysql::connection >();
}

//-----------------------------------------------------------------------------
