//-----------------------------------------------------------------------------

#include <mssql/error.h>
#include <sqlext.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

[[noreturn]] void throw_error( const std::string & operation, const char * error )
{
   throw std::runtime_error( operation + ": " + error );
}

//-----------------------------------------------------------------------------

void check_status( const char * operation, SQLHANDLE handle, SQLSMALLINT type, RETCODE rc )
{
   if ( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO )
   {
      std::string message;

      if ( rc != SQL_INVALID_HANDLE )
      {
         SQLSMALLINT rec = 0;
         SQLINTEGER  error;
         SQLCHAR     msg[ 1000 ];
         SQLCHAR     state[ SQL_SQLSTATE_SIZE + 1 ];

         while ( SQLGetDiagRec( type, handle, ++rec, state, &error, msg, sizeof( msg ), nullptr ) == SQL_SUCCESS )
         {
             message += reinterpret_cast< char * >( msg );
             message += "\n";
         }
      }
      else
         message = "Invalid handle";

      throw_error( operation, message.c_str() );
   }
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------
