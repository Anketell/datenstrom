//-----------------------------------------------------------------------------

#include <firebird/error.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

[[noreturn]] void throw_error( const std::string & operation, const char * error )
{
   throw std::runtime_error( operation + ": " + error );
}

//-----------------------------------------------------------------------------

void check_status( const char * operation, const ISC_STATUS * status )
{
   if ( status[ 0 ] == 1 && status[ 1 ] )
   {
      std::string errors;

      char error[ 512 ];

      while ( fb_interpret( error, sizeof( error ), &status ) )
      {
         errors += error;
         errors += "\n";
      }

      throw_error( operation, errors.c_str() );
   }
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

