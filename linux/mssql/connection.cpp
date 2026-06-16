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
                                                  int                 port,
                                                  int                 timeout )
{
   std::stringstream ss;

   ss << "Driver={SQL Server}; "
      << "Server=" << server;

   if ( !instance.empty() )
      ss << "\\" << instance;

   ss << "," << port << "; "
      << "TrustServerCertificate=yes; "
      << "Trusted_Connection=yes; "
      << "Connect Timeout=" << timeout << ";";

   return ss.str();
}

//-----------------------------------------------------------------------------

std::string connection::create_connection_string( const std::string & user_id,
                                                  const std::string & password,
                                                  const std::string & server,
                                                  const std::string & instance,
                                                  int                 port,
                                                  int                 timeout )
{
   std::stringstream ss;

   ss << "Driver={SQL Server}; "
      << "Server=" << server;

   if ( !instance.empty() )
      ss << "\\" << instance;

   ss << "," << port << "; "
      << "TrustServerCertificate=yes; "
      << "Connect Timeout=" << timeout << "; "
      << "UID=" << user_id << "; "
      << "PWD=" << password << "; ";

   return ss.str();
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & server,
                        const std::string & instance,
                        const std::string & variant,
                        int                 port,
                        int                 timeout )
{
   m_connection_string = create_connection_string( server, instance, port, timeout );
   set_variant( variant );
   init( m_connection_string );
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & user_id,
                        const std::string & password,
                        const std::string & server,
                        const std::string & instance,
                        const std::string & variant,
                        int                 port,
                        int                 timeout )
{
   m_connection_string = create_connection_string( user_id,
                                                   password,
                                                   server,
                                                   instance,
                                                   port,
                                                   timeout );
   set_variant( variant );
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

void connection::set_variant( std::string version )
{
   static const std::map< const std::string, use_variant_t > variant_map =
   {
      { "server", &connection::use_server },
      { "azure",  &connection::use_azure  }
   };

   for ( auto it = version.begin(); it != version.end(); it++ )
      *it = std::tolower( *it );

   m_use_variant = &connection::use_unknown;

   for ( auto it = variant_map.begin(); it != variant_map.end(); it++ )
   {
      if ( version.find( it->first ) != std::string::npos )
      {
         m_use_variant = it->second;
         break;
      }
   }
}

//-----------------------------------------------------------------------------

void connection::init( const std::string & connection_string )
{
   static constexpr char operation[] = "MSSQL initializing connection";

   try
   {
      RETCODE rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLSetEnvAttr( m_henv, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER )SQL_OV_ODBC3, 0 );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      init_connection( connection_string );
   }
   catch ( ... )
   {
      cleanup();
      throw;
   }
}

//-----------------------------------------------------------------------------

void connection::init_connection( const std::string & connection_string )
{
   static constexpr char operation[] = "MSSQL initializing connection";

   RETCODE rc = SQLAllocHandle( SQL_HANDLE_DBC, m_henv, &m_hdbc );
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
   check_status( operation, m_stmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

void connection::cleanup( void )
{
   cleanup_connection();

   if ( m_henv )
   {
      SQLFreeHandle( SQL_HANDLE_ENV, m_henv );
      m_henv = nullptr;
   }
}

//-----------------------------------------------------------------------------

void connection::cleanup_connection( void )
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

   ( this->*m_use_variant )( name );
}

//-----------------------------------------------------------------------------

void connection::use_unknown( const std::string & name )
{
   std::string version = operator()( "SELECT @@VERSION" );
   set_variant( version );

   use( name );
}

//-----------------------------------------------------------------------------

void connection::use_server( const std::string & name )
{
   std::string query = "USE " + name;

   RETCODE rc = SQLExecDirect( m_stmt, sql_char( query.c_str() ), sql_int( query.length() ) );
   check_status( "MSSQL create database", m_hdbc, SQL_HANDLE_DBC, rc );

   m_database = name;
}

//-----------------------------------------------------------------------------

void connection::use_azure( const std::string & name )
{
   try
   {
      cleanup_connection();
      init_connection( m_connection_string + " Database=" + name + ";" );
   }
   catch ( ... )
   {
      cleanup_connection();
      init_connection( m_connection_string );
      throw;
   }

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
   ds::db::transaction txn( *this );

   for ( auto & statement : statement_enum( query ) )
   {
      RETCODE rc = SQLExecDirect( m_stmt, sql_char( statement.from ), sql_int( statement.len ) );
      check_status( "MSSQL execute batch", m_stmt, SQL_HANDLE_STMT, rc );
   }
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   static constexpr char operation[] = "MSSQL begin transaction";

   if ( m_txn_count == 0 )
   {
      static constexpr char query[] = "BEGIN TRANSACTION";

      RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
      check_status( operation, m_stmt, SQL_HANDLE_STMT, rc );
   }
   m_txn_count++;
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   static constexpr char query[] = "COMMIT";

   if ( m_txn_count < 2 )
   {
      RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
      check_status( "MSSQL commit transaction", m_stmt, SQL_HANDLE_STMT, rc );
   }
   m_txn_count--;
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   static constexpr char query[] = "ROLLBACK TRANSACTION";

   if ( m_txn_count < 2 )
   {
      RETCODE rc = SQLExecDirect( m_stmt, sql_char( query ), SQL_NTS );
      check_status( "MSSQL rollback transaction", m_stmt, SQL_HANDLE_STMT, rc );
   }
   m_txn_count--;
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
