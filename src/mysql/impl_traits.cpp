//-----------------------------------------------------------------------------

#include <mysql/impl_traits.h>
#include <db/connect_params.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

constexpr char impl_traits< mysql::connection >::TYPE[];

//-----------------------------------------------------------------------------

impl * impl_traits< mysql::connection >::construct( const db::connect_params_t & params )
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
