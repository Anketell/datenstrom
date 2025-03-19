//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/error.h>
#include <sqlext.h>
#include <sstream>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds::mssql
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
      std::stringstream message;

      if ( rc != SQL_INVALID_HANDLE )
      {
         SQLSMALLINT rec = 0;
         SQLINTEGER  error;
         SQLCHAR     msg[ 1000 ];
         SQLCHAR     state[ SQL_SQLSTATE_SIZE + 1 ];
         SQLLEN      count = 0;

         SQLGetDiagField( SQL_HANDLE_STMT, handle, 0, SQL_DIAG_NUMBER, &count, 0, 0 );

         if ( count )
         {
            while ( SQLGetDiagRec( type, handle, ++rec, state, &error, msg, sizeof( msg ), nullptr ) == SQL_SUCCESS )
               message << "(" << error << ") " << reinterpret_cast< char * >( msg ) << std::endl;
         }
         else
            message << "No details (" << rc << ")";
      }
      else
         message << "Invalid handle";

      throw_error( operation, message.str().c_str() );
   }
}

//-----------------------------------------------------------------------------

}
