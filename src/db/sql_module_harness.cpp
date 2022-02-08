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

}

//-----------------------------------------------------------------------------

extern "C" void enroll_sql( ds::db::sql::map_t & sql_map )
{
   sql_map.insert( { ds::db::sql::module_type, ds::db::sql::lookup } );
}

//-----------------------------------------------------------------------------
