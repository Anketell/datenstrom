//-----------------------------------------------------------------------------

#include <db/enroll.h>
#include <util/filesys.h>
#include <util/module.h>

//-----------------------------------------------------------------------------

extern "C"
{
   typedef void ( * enroll_t )( ds::db::factory & factory );
}

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

void enroll_module( factory & factory, const std::string & path )
{
   util::module module( path );

   if ( !module )
      return;

   enroll_t enroll_module = module.symbol< enroll_t >( "enroll" );

   if ( enroll_module )
      enroll_module( factory );
   else
      module.close();
}

//-----------------------------------------------------------------------------

void enroll_directory( factory & factory, const std::string & path )
{
   for ( auto file_path : util::filesys::find( path, { util::module::PATTERN } ) )
      enroll_module( factory, file_path );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
