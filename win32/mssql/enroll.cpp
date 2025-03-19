//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/factory.h>
#include <mssql/connection.h>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

template<> impl * factory_helper< mssql::connection >::construct( const connect_params_t & params )
{
   auto server   = params[ "server" ];
   auto path     = params[ "path" ];
   auto port_str = params[ "port" ];
   auto database = params[ "database" ];
   auto username = params[ "username" ];
   auto password = params[ "password" ];

   if ( server.empty() )
      throw std::invalid_argument( "Connect string does not specify server" );

   int port = 1433;
   if ( !port_str.empty() )
      port = atoi( port_str.c_str() );

   impl * db;

   if ( !username.empty() )
      db = new mssql::connection( username, password, server, path, port );
   else
      db = new mssql::connection( server, path, port );

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

extern "C" void enroll_db( ds::db::factory & factory )
{
   factory.register_impl< ds::mssql::connection >();
}

//-----------------------------------------------------------------------------
