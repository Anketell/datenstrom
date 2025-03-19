//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/enroll.h>
#include <dsutil/filesys.h>
#include <dsutil/module.h>

//-----------------------------------------------------------------------------

extern "C"
{
   typedef void ( * enroll_t )( ds::db::factory & factory );
}

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

void enroll_module( factory & factory, const std::string & path )
{
   module module( path );

   if ( !module )
      return;

   enroll_t enroll_module = module.symbol< enroll_t >( "enroll_db" );

   if ( enroll_module )
      enroll_module( factory );
   else
      module.close();
}

//-----------------------------------------------------------------------------

void enroll_directory( factory & factory, const std::string & path )
{
   for ( auto file_path : filesys::find( path, { module::PATTERN } ) )
      enroll_module( factory, file_path );
}

//-----------------------------------------------------------------------------

}
