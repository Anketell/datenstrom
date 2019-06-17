//-----------------------------------------------------------------------------

#ifndef DS_SQLITE_CONSTRUCTOR_H
#define DS_SQLITE_CONSTRUCTOR_H

//-----------------------------------------------------------------------------

#include <sqlite/constructor.h>
#include <db/impl.h>
#include <db/connect_string.h>
#include <sqlite/connection.h>

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
      throw std::invalid_argument( "Unspecified location" );

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

#endif
