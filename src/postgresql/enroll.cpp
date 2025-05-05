//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/factory.h>
#include <postgresql/connection.h>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

template<> impl * factory_helper< pg::connection >::construct( const connect_params_t & params )
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

   int port = 5432;
   if ( !port_str.empty() )
      port = atoi( port_str.c_str() );

   impl * db = new pg::connection( username, password, server, port );

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
   factory.register_impl< ds::pg::connection >();
}

//-----------------------------------------------------------------------------
