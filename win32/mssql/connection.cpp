//-----------------------------------------------------------------------------

#include <mssql/connection.h>
#include <mssql/statement.h>
#include <mssql/error.h>
#include <sqlext.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

std::string connection::create_connection_string( const std::string & server, 
                                                  int                 port )
{
   std::stringstream ss;
   
   ss << "Driver={SQL Server}; "
       << "Server=" << server << "," << port << "; "
       << "Trusted_Connection=yes;";
   
   return ss.str();;
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & server, int port )
{
   std::string connection_string = create_connection_string( server, port );
   init( connection_string );
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
   static const char operation[] = "mssql initializing connection";

   try
   {
      RETCODE rc = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER )SQL_OV_ODBC3, 0);
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      rc = SQLAllocHandle( SQL_HANDLE_DBC, m_henv, &m_hdbc );
      check_status( operation, m_henv, SQL_HANDLE_ENV, rc );

      const SQLCHAR * conn_str = reinterpret_cast< const SQLCHAR * >( connection_string.c_str() );

      rc = SQLDriverConnect( m_hdbc,
                             nullptr,
                             const_cast< SQLCHAR * >( conn_str ),
                             static_cast< SQLSMALLINT >( connection_string.length() ),
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

void connection::create( const std::string & name )
{
   std::string query = "CREATE DATABASE " + name;

   const SQLCHAR * query_str = reinterpret_cast< const SQLCHAR * >( query.c_str() );
   RETCODE rc = SQLExecDirect( m_stmt, const_cast< SQLCHAR * >( query_str ), SQL_NTS );
   check_status( "mssql create database", m_hdbc, SQL_HANDLE_DBC, rc );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   if ( name == m_database )
      return;

   std::string query = "USE " + name;

   const SQLCHAR * query_str = reinterpret_cast< const SQLCHAR * >( query.c_str() );
   RETCODE rc = SQLExecDirect( m_stmt, const_cast< SQLCHAR * >( query_str ), SQL_NTS );
   check_status( "mssql use database", m_hdbc, SQL_HANDLE_DBC, rc );

   m_database = name;
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   if ( name == m_database )
      use();

   std::string query = "DROP DATABASE " + name;

   const SQLCHAR * query_str = reinterpret_cast< const SQLCHAR * >( query.c_str() );
   RETCODE rc = SQLExecDirect( m_stmt, const_cast< SQLCHAR * >( query_str ), SQL_NTS );

   return rc == SQL_SUCCESS;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   throw ds::Not_implemented();
//   return db::statement( std::make_shared< statement >( query, parameters ) );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   const SQLCHAR * query_str = reinterpret_cast< const SQLCHAR * >( query.c_str() );
   RETCODE rc = SQLExecDirect( m_stmt, const_cast< SQLCHAR * >( query_str ), SQL_NTS );
   check_status( "mssql execute batch", m_hdbc, SQL_HANDLE_DBC, rc );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
    throw ds::Not_implemented();
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
