//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/sql_map.h>
#include <dsutil/module.h>
#include <dsutil/filesys.h>

//-----------------------------------------------------------------------------

namespace ds::db::sql
{

//-----------------------------------------------------------------------------

void enroll_module( module_map_t & module_map, const std::string & path )
{
   ds::module module( path );

   if ( !module )
      return;

   enroll_t enroll_module = module.symbol< enroll_t >( "enroll_sql" );

   if ( enroll_module )
      enroll_module( module_map );
   else
      module.close();
}

//-----------------------------------------------------------------------------

void enroll_directory( module_map_t & module_map, const std::string & path )
{
   for ( auto file_path : ds::filesys::find( path, { ds::module::PATTERN } ) )
      enroll_module( module_map, file_path );
}

//-----------------------------------------------------------------------------

}
