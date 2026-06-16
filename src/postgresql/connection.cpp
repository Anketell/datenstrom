//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <postgresql/connection.h>
#include <postgresql/positional_statement.h>
#include <postgresql/named_statement.h>
#include <postgresql/error.h>
#include <sstream>
#include <regex>
#include <libpq-fe.h>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

connection::connection( const std::string & user_id,
                        const std::string & password,
                        const std::string & server, int port ) :
m_conn( nullptr ),
m_user_id( user_id ),
m_password( password ),
m_server( server ),
m_port( std::to_string( port ) )
{
}

//-----------------------------------------------------------------------------

void connection::detach( void )
{
   if ( m_conn )
   {
      PQfinish( m_conn );
      m_conn      = nullptr;
      m_txn_count = 0;
   }
}

//-----------------------------------------------------------------------------

connection::~connection( void )
{
   detach();
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return TYPE;
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   static constexpr char operation[] = "PostgreSQL create database";

   use();

   PGresult * res = PQexec( m_conn, ( "CREATE DATABASE " + name ).c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::string message = PQerrorMessage( m_conn );
      PQclear( res );
      throw_error( operation, message.c_str() );
   }
   PQclear( res );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   detach();

   m_conn = PQsetdbLogin( m_server.c_str(),
                          m_port.c_str(),
                          nullptr,
                          nullptr,
                          name.c_str(),
                          m_user_id.c_str(),
                          m_password.c_str() );

   if ( PQstatus( m_conn ) != CONNECTION_OK )
   {
      std::string message = PQerrorMessage( m_conn );
      detach();
      throw_error( "PostgreSQL connection failed", message.c_str() );
   }
}

//-----------------------------------------------------------------------------

void connection::use( void )
{
   if ( !m_conn )
      use( "postgres" );
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   static constexpr char operation[] = "PostgreSQL drop database";
   static std::regex     no_db( "^ERROR: *database .* does not exist\n$" );

   if ( m_conn && PQdb( m_conn ) == name )
      detach();

   use();

   PGresult * res = PQexec( m_conn, ( "DROP DATABASE " + name ).c_str() );

   bool deleted = true;

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::string message = PQerrorMessage( m_conn );

      if ( !std::regex_search( message, no_db ))
      {
         PQclear( res );
         throw_error( operation, message.c_str() );
      }

      deleted = false;
   }
   PQclear( res );

   return deleted;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   if ( parameters.empty() )
      return db::statement( std::make_shared< positional_statement >( m_conn, query ) );

   return db::statement( std::make_shared< named_statement >( m_conn, query, parameters ) );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   static constexpr char operation[] = "PostgreSQL execute batch";

   PGresult * res = PQexec( m_conn, query.c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::string message = PQerrorMessage( m_conn );
      PQclear( res );
      throw_error( operation, message.c_str() );
   }
   PQclear( res );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   static constexpr char operation[] = "PostgreSQL begin transaction";

   if ( m_txn_count == 0 )
   {
      PGresult * res = PQexec( m_conn, "BEGIN TRANSACTION" );

      if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
      {
         std::string message = PQerrorMessage( m_conn );
         PQclear( res );
         throw_error( operation, message.c_str() );
      }
      PQclear( res );
   }
   m_txn_count++;
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   static constexpr char operation[] = "PostgreSQL commit transaction";

   if ( m_txn_count < 2 )
   {
      if ( m_txn_count == 0 )
         throw_error( operation, "No transaction to commit" );

      PGresult * res = PQexec( m_conn, "COMMIT TRANSACTION" );

      if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
      {
         std::string message = PQerrorMessage( m_conn );
         PQclear( res );
         throw_error( operation, message.c_str() );
      }
      PQclear( res );
   }
   m_txn_count--;
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   static constexpr char operation[] = "PostgreSQL rollback transaction";

   if ( m_txn_count < 2 )
   {
      if ( m_txn_count == 0 )
         throw_error( operation, "No transaction to rollback" );

      PGresult * res = PQexec( m_conn, "ROLLBACK TRANSACTION" );

      if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
      {
         std::string message = PQerrorMessage( m_conn );
         PQclear( res );
         throw_error( operation, message.c_str() );
      }
      PQclear( res );
   }
   m_txn_count--;
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   constexpr char operation[] = "PostgreSQL create savepoint";

   if ( name.empty() )
      throw_error( operation, "savepoint name not specified" );

   std::string command = "SAVEPOINT \"" + name + "\"";

   PGresult * res = PQexec( m_conn, command.c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::stringstream ss;

      ss << operation << " \"" << name << "\"";

      std::string message = PQerrorMessage( m_conn );
      PQclear( res );
      throw_error( ss.str().c_str(), message.c_str() );
   }
   PQclear( res );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   constexpr char operation[] = "PostgreSQL release savepoint";

   if ( name.empty() )
      throw_error( operation, "savepoint name not specified" );

   const char * message = PQerrorMessage( m_conn );
   if ( message && *message )
      return;

   std::string command = "RELEASE SAVEPOINT \"" + name + "\"";

   PGresult * res = PQexec( m_conn, command.c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::stringstream ss;

      ss << operation << " \"" << name << "\"";

      std::string message = PQerrorMessage( m_conn );
      PQclear( res );
      throw_error( ss.str().c_str(), message.c_str() );
   }
   PQclear( res );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   constexpr char operation[] = "PostgreSQL rollback to savepoint";

   if ( name.empty() )
      throw_error( operation, "savepoint name not specified" );

   std::string command = "ROLLBACK TO SAVEPOINT \"" + name + "\"";

   PGresult * res = PQexec( m_conn, command.c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::stringstream ss;

      ss << operation << " \"" << name << "\"";

      std::string message = PQerrorMessage( m_conn );
      PQclear( res );
      throw_error( ss.str().c_str(), message.c_str() );
   }
   PQclear( res );
}

//-----------------------------------------------------------------------------

}
