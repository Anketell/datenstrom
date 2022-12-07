//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/connection.h>
#include <sqlite/statement.h>
#include <sqlite/error.h>
#include <dsutil/filesys.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

connection::connection( const std::string & path ) :
m_path( path )
{
}

//-----------------------------------------------------------------------------

connection::~connection( void )
{
   close();
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return TYPE;
}

//-----------------------------------------------------------------------------

void connection::close( void )
{
   if ( m_db )
   {
      sqlite3_close( m_db );
      m_db = nullptr;
   }
}

//-----------------------------------------------------------------------------

std::string connection::get_full_path( const std::string & name ) const
{
   return m_path + "/" + name + ".db";
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   static constexpr char operation[] = "SQLite create database";

   std::string path = get_full_path( name );

   if ( filesys::exists( path.c_str() ) )
      throw_error( operation, ( path + " exists" ).c_str() );

   close();

   int rc = sqlite3_open_v2( path.c_str(),
                             &m_db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             nullptr );
   if ( rc )
   {
      close();
      throw_error( operation, rc );
   }
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   close();

   int rc = sqlite3_open_v2( get_full_path( name ).c_str(),
                             &m_db,
                             SQLITE_OPEN_READWRITE,
                             nullptr );
   if ( rc )
   {
      close();
      throw_error( "SQLite use database", rc );
   }
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   close();

   try
   {
      filesys::remove( get_full_path( name ).c_str() );
   }
   catch ( std::exception & e )
   {
      throw_error( "SQLite drop database", e.what() );
   }

   return true;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   return db::statement( std::make_shared< statement >( m_db, query, parameters ) );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   int rc = sqlite3_exec( m_db, query.c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite execute batch", rc );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   int rc = sqlite3_exec( m_db, "BEGIN TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite begin transaction", rc );
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   int rc = sqlite3_exec( m_db, "COMMIT TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite commit transaction", rc );
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   int rc = sqlite3_exec( m_db, "ROLLBACK TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite rollback transaction", rc );
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   std::string command( "SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite create savepoint " + name, rc );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   std::string command( "RELEASE SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
       throw_error( "SQLite release savepoint " + name, rc );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   std::string command( "ROLLBACK TO SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite rollback to savepoint " + name, rc );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
