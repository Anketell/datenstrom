//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
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
      throw_error( "MySQL create database", mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   std::string sql = "USE " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL use database", mysql_error( &m_mysql ) );
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

      throw_error( "MySQL drop database", mysql_error( &m_mysql ) );
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

void connection::execute_batch( const std::string & query )
{
   static constexpr char operation[] = "MySQL execute batch";

   int rc = mysql_real_query( &m_mysql, query.c_str(), query.length() );
   if ( rc )
      throw_error( operation, mysql_error( &m_mysql ) );

   do
   {
      rc = mysql_next_result( &m_mysql );
      if ( rc > 0 )
         throw_error( operation, mysql_error( &m_mysql ) );
   }
   while ( rc == 0 );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   static constexpr char operation[] = "MySQL begin transaction";

   if ( m_transaction )
      throw_error( operation, "nested transaction unsupported" );

   std::string sql = "BEGIN";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( operation, mysql_error( &m_mysql ) );

   m_transaction = true;
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   static constexpr char operation[] = "MySQL commit transaction";
   
   if ( !m_transaction )
      throw_error( operation, "no transaction to commit" );

   std::string sql = "COMMIT";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( operation, mysql_error( &m_mysql ) );

   m_transaction = false;
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   static constexpr char operation[] = "MySQL rollback transaction";
   
   if ( !m_transaction )
      throw_error( operation, "no transaction to rollback" );

   std::string sql = "ROLLBACK";

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( operation, mysql_error( &m_mysql ) );

   m_transaction = false;
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   std::string sql = "SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL create savepoint " + name, mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   std::string sql = "RELEASE SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL release savepoint " + name, mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   std::string sql = "ROLLBACK TO SAVEPOINT " + name;

   int rc = mysql_real_query( &m_mysql, sql.c_str(), sql.length() );
   if ( rc )
      throw_error( "MySQL rollback to savepoint " + name, mysql_error( &m_mysql ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
