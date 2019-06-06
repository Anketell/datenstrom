//-----------------------------------------------------------------------------

#include <mysql/database.h>
#include <mysql/statement.h>
#include <mysql/prepared_statement.h>
#include <mysql/error.h>
#include <iostream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

database::database( const std::string & name,
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

database::database( const std::string & server,
                    const std::string & username,
                    const std::string & password,
                    uint16_t            port ) :
database( "", server, username, password, port )
{
}

//-----------------------------------------------------------------------------

database::~database( void )
{
   mysql_close( &m_mysql );
}

//-----------------------------------------------------------------------------

void database::create( const std::string & name )
{
   std::string sql = "CREATE DATABASE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL create database", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void database::use( const std::string & name )
{
   std::string sql = "USE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL use database", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

bool database::drop( const std::string & name )
{
   std::string sql = "DROP DATABASE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
   {
      if ( mysql_errno( &m_mysql ) == 1008 )
         return false;

      throw_error( "MySQL drop database", mysql_error( &m_mysql ) );
   }

   return true;
}

//-----------------------------------------------------------------------------

db::statement database::operator()( const std::string     & query,
                                    const db::name_list_t & parameters )
{
   if ( parameters.empty() )
      return db::statement( std::make_shared< prepared_statement >( m_mysql, query ) );

   return db::statement( std::make_shared< statement >( m_mysql, query, parameters ) );
}

//-----------------------------------------------------------------------------

void database::begin_transaction( void )
{
}

//-----------------------------------------------------------------------------

void database::commit_transaction( void )
{
}

//-----------------------------------------------------------------------------

void database::rollback_transaction( void )
{

}

//-----------------------------------------------------------------------------

void database::savepoint( const std::string & )
{
}

//-----------------------------------------------------------------------------

void database::release_savepoint( const std::string & )
{
}

//-----------------------------------------------------------------------------

void database::rollback_to_savepoint( const std::string & )
{
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
