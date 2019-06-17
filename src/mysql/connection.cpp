//-----------------------------------------------------------------------------

#include <mysql/connection.h>
#include <mysql/positional_statement.h>
#include <mysql/named_statement.h>
#include <mysql/error.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

connection::connection( const std::string & name,
                        const std::string & server,
                        const std::string & username,
                        const std::string & password,
                        uint16_t            port )
{
   mysql_init( &m_mysql );

   MYSQL * rc = mysql_real_connect( &m_mysql,
                                    server.c_str(),
                                    username.c_str(),
                                    password.c_str(),
                                    name.c_str(),
                                    port,
                                    nullptr,
                                    CLIENT_MULTI_STATEMENTS );

   if ( !rc )
   {
      mysql_close( &m_mysql );
      throw_error( "MySQL connection failed", mysql_error( &m_mysql ) );
   }
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & server,
                        const std::string & username,
                        const std::string & password,
                        uint16_t            port ) :
connection( "", server, username, password, port )
{
}

//-----------------------------------------------------------------------------

connection::~connection( void )
{
   mysql_close( &m_mysql );
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return TYPE;
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   std::string sql = "CREATE DATABASE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL create connection", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   std::string sql = "USE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL use connection", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   std::string sql = "DROP DATABASE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
   {
      if ( mysql_errno( &m_mysql ) == 1008 )
         return false;

      throw_error( "MySQL drop connection", mysql_error( &m_mysql ) );
   }

   return true;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   if ( parameters.empty() )
      return db::statement( std::make_shared< positional_statement >( m_mysql, query ) );

   return db::statement( std::make_shared< named_statement >( m_mysql, query, parameters ) );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   std::string sql = "BEGIN";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL begin transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   std::string sql = "COMMIT";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL commit transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   std::string sql = "ROLLBACK";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL rollback transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   std::string sql = "SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL create savepoint " + name + " transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   std::string sql = "RELEASE SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL release savepoint " + name + " transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   std::string sql = "ROLLBACK TO SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL rollback to savepoint " + name + " transaction", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
