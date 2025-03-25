//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <mssql/connection.h>
#include <mssql/positional_statement.h>
#include <mssql/named_statement.h>
#include <mssql/statement_enum.h>
#include <mssql/error.h>
#include <sqlext.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

std::string connection::create_connection_string( const std::string & server,
                                                  const std::string & instance,
                                                  int                 port )
{
   std::stringstream ss;

   ss << "Driver={SQL Server}; "
      << "Server=" << server;

   if ( !instance.empty() )
      ss << "\\" << instance;

   if ( port != 1433 )
      ss << "," << port;

    ss << "; Trusted_Connection=yes;";

   return ss.str();
}

//-----------------------------------------------------------------------------

std::string connection::create_connection_string( const std::string & user_id,
                                                  const std::string & password,
                                                  const std::string & server,
                                                  const std::string & instance,
                                                  int                 port )
{
   std::stringstream ss;

   ss << "Driver=FreeTDS; "
      << "Server=" << server;

//   ss << "Driver={ODBC Driver 18 for SQL Server}; "
//      << "Server=" << server;

   if ( !instance.empty() )
      ss << "\\" << instance;

//   if ( port != 1433 )
//      ss << "," << port;

//   ss << "; port=1433; DS_Version=8.0; Driver=/usr/lib64/libtdsodbc.so";
   ss << "; port=1433";

//   ss << "; TrustServerCertificate=yes";


   ss << "; "
      << "UID=" << user_id << "; "
      << "PWD=" << password << "; ";

   return ss.str();
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & server, const std::string & instance, int port )
{
   m_connection_string = create_connection_string( server, instance, port );
   init( m_connection_string );
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & user_id,
                        const std::string & password,
                        const std::string & server, const std::string & instance, int port )
{
   m_connection_string = create_connection_string( user_id, password, server, instance, port );
   init( m_connection_string );
}

//-----------------------------------------------------------------------------

connection::~connection( void )
{
   cleanup();
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return TYPE;
}

//-----------------------------------------------------------------------------

void connection::init( const std::string& connection_string )
{
   static constexpr char operation[] = "MSSQL initializing connection";

   try
   {
      RETCODE rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLSetEnvAttr( m_henv, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER )SQL_OV_ODBC3, 0 );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLAllocHandle( SQL_HANDLE_DBC, m_henv, &m_hdbc );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLDriverConnect( m_hdbc,
                             nullptr,
                             sql_char( connection_string.c_str() ),
                             sql_smint( connection_string.length() ),
                             nullptr,
                             0,
                             nullptr,
                             SQL_DRIVER_NOPROMPT );

      check_status( operation, m_hdbc, SQL_HANDLE_DBC, rc );

      rc = SQLAllocHandle( SQL_HANDLE_STMT, m_hdbc, &m_stmt );
      check_status( operation, m_hdbc, SQL_HANDLE_DBC, rc );
   }
   catch (...)
   {
      cleanup();
      throw;
   }
}

//-----------------------------------------------------------------------------

void connection::cleanup( void )
{
   if ( m_stmt )
   {
      SQLFreeHandle( SQL_HANDLE_STMT, m_stmt );
      m_stmt = nullptr;
   }

   if ( m_hdbc )
   {
      SQLDisconnect( m_hdbc );
      SQLFreeHandle( SQL_HANDLE_DBC, m_hdbc );
      m_hdbc = nullptr;
   }

   if ( m_henv )
   {
      SQLFreeHandle( SQL_HANDLE_ENV, m_henv );
      m_henv = nullptr;
   }
}

//-----------------------------------------------------------------------------

void connection::guard( guarded_fn fn )
{
   if ( !m_transactions )
   {
      db::transaction transaction( *this );
      fn();
   }
   else
      fn();
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   std::string query = "CREATE DATABASE " + name;

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );
   check_status( "MSSQL create database", m_hdbc, SQL_HANDLE_DBC, rc );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   if ( name == m_database )
      return;

   std::string query = "USE " + name;

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );
   check_status( "MSSQL create database", m_hdbc, SQL_HANDLE_DBC, rc );
/*
   // Auzre doesn't support using a database not specified in the
   // connection string so try to reconnect to the database.

   if ( rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO )
   {
      cleanup();
      init( m_connection_string + " Database=" + name + ";" );
   }
*/
   m_database = name;
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   if ( name == m_database )
      use();

   std::string query = "DROP DATABASE " + name;

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );

   return rc == SQL_SUCCESS;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   if ( parameters.empty() )
      return db::statement( std::make_shared< positional_statement >( m_hdbc, query ) );

   return db::statement( std::make_shared< named_statement >( m_hdbc, query, parameters ) );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   guard( [ & ]()
   {
      for ( auto & statement : statement_enum( query ) )
      {
         RETCODE rc = SQLExecDirect( m_stmt, sql_char( statement.from ), sql_int( statement.len ) );
         check_status( "MSSQL execute batch", m_stmt, SQL_HANDLE_STMT, rc );
      }
   } );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   static constexpr char operation[] = "MSSQL begin transaction";

   if ( m_transactions )
      throw_error( operation, "failed nested transactions" );

   static constexpr char query[] = "BEGIN TRANSACTION";

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
   check_status( operation, m_stmt, SQL_HANDLE_STMT, rc );

   m_transactions++;
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   static constexpr char query[] = "COMMIT";

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
   check_status( "MSSQL commit transaction", m_stmt, SQL_HANDLE_STMT, rc );

   m_transactions--;
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   static constexpr char query[] = "ROLLBACK TRANSACTION";

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
   check_status( "MSSQL rollback transaction", m_stmt, SQL_HANDLE_STMT, rc );

   m_transactions--;
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   std::string query = "SAVE TRANSACTION \"" + name + "\"";

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );
   check_status( "MSSQL savepoint", m_stmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   // N/A not supported by MSSQL
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   std::string query = "ROLLBACK TRANSACTION \"" + name + "\"";

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );
   check_status( "MSSQL rollback savepoint", m_stmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

}
