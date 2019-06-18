//-----------------------------------------------------------------------------

#include <db/enroll.h>
#include <dlfcn.h>
#include <dirent.h>

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
   void * handle = dlopen( path.c_str(), RTLD_LAZY | RTLD_LOCAL | RTLD_NODELETE );

   if ( !handle )
      return;

   enroll_t enroll_fn = reinterpret_cast< enroll_t >( dlsym( handle, "enroll" ) );

   if ( enroll_fn )
      enroll_fn( factory );

   dlclose( handle );
}

//-----------------------------------------------------------------------------

void enroll_directory( factory & factory, const std::string & path )
{
   dirent ** files;

   int n = scandir( path.c_str(), &files, nullptr, versionsort );

   if ( n == -1 )
      throw std::runtime_error( "Directory scan filed" );

   for ( int i = 0; i < n; i++ )
   {
      dirent * file = files[ i ];
      enroll_module( factory, path + "/" + file->d_name );
      free( file );
   }

   free( files );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
