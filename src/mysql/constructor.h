//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_CONSTRUCTOR_H
#define DS_MYSQL_CONSTRUCTOR_H

//-----------------------------------------------------------------------------

#include <mysql/constructor.h>
#include <db/impl.h>
#include <db/connect_string.h>
#include <mysql/connection.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

template<> impl * constructor< mysql::connection > ( const db::connect_params_t & params )
{
   auto location = params[ "location" ];
   auto port     = params[ "port" ];
   auto username = params[ "username" ];
   auto password = params[ "password" ];
   auto database = params[ "database" ];

   if ( location.empty() )
      throw std::invalid_argument( "Unspecified location" );

   if ( username.empty() )
      throw std::invalid_argument( "Unspecified username" );

   if ( port.empty() )
      port = "3306";

   return new mysql::connection( database, location, username, password, atoi( port.c_str() ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
