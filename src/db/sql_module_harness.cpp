//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/sql_module_harness.h>
#include <db/context.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds::db::sql
{

//-----------------------------------------------------------------------------

const char * lookup( const std::string & key )
{
   auto it = sql_map.find( key );
   if ( it == sql_map.end() )
      return nullptr;
   return it->second;
}

//-----------------------------------------------------------------------------

void keys( keyset_t & keyset )
{
   for ( auto it : sql_map )
      keyset.insert( it.first );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

extern "C" void enroll_sql( ds::db::sql::module_map_t & module_map )
{
   module_map.insert(
   {
      ds::db::sql::module_type,
      {
         ds::db::sql::lookup,
         ds::db::sql::keys
      }
   } );
}

//-----------------------------------------------------------------------------
