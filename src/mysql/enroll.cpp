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

class connect_params_t;

//-----------------------------------------------------------------------------

template<> struct factory_helper< mysql::connection >
{
   static const char * type( void ) { return mysql::connection::TYPE; }
   static impl * construct( const db::connect_params_t & params );
};

//-----------------------------------------------------------------------------

impl * factory_helper< mysql::connection >::construct( const db::connect_params_t & params )
{
   auto location = params[ "location" ];
   auto port_str = params[ "port" ];
   auto username = params[ "username" ];
   auto password = params[ "password" ];
   auto database = params[ "database" ];

   if ( location.empty() )
      throw std::invalid_argument( "Connect string does not specify location" );

   if ( username.empty() )
      throw std::invalid_argument( "Connect string does not specify username" );

   int port = 3306;
   if ( !port_str.empty() )
      port = atoi( port_str.c_str() );

   return new mysql::connection( database, location, username, password, port );
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
