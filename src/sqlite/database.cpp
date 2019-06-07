//-----------------------------------------------------------------------------

#include <sqlite/database.h>
#include <sqlite/statement.h>
#include <sqlite/error.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

constexpr char database::name[];

//-----------------------------------------------------------------------------

database::database( const std::string & path ) :
m_path( path )
{
}

//-----------------------------------------------------------------------------

database::~database( void )
{
   sqlite3_close( m_db );
}

//-----------------------------------------------------------------------------

void database::close( void )
{
   if ( m_db )
   {
      sqlite3_close( m_db );
      m_db = nullptr;
   }
}

//-----------------------------------------------------------------------------

std::string database::get_full_path( const std::string & name ) const
{
   return m_path + "/" + name + ".db";
}

//-----------------------------------------------------------------------------

void database::create( const std::string & name )
{
   static constexpr char operation[] = "SQLite create database";

   std::string path = get_full_path( name );

   if ( access( path.c_str(), F_OK ) == 0 )
   {
      throw_error( operation, ( path + " exists" ).c_str() );
   }

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

void database::use( const std::string & name )
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

bool database::drop( const std::string & name )
{
   if ( unlink( get_full_path( name ).c_str() ) != 0 )
   {
      if ( errno == ENOENT )
         return false;

      throw_error( "SQLite drop database", strerror( errno ) );
   }

   return true;
}

//-----------------------------------------------------------------------------

db::statement database::operator()( const std::string     & query,
                                    const db::name_list_t & parameters )
{
   return db::statement( std::make_shared< statement >( m_db, query, parameters ) );
}

//-----------------------------------------------------------------------------

void database::begin_transaction( void )
{
   int rc = sqlite3_exec( m_db, "BEGIN TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite begin transaction", rc );
}

//-----------------------------------------------------------------------------

void database::commit_transaction( void )
{
   int rc = sqlite3_exec( m_db, "COMMIT TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite commit transaction", rc );
}

//-----------------------------------------------------------------------------

void database::rollback_transaction( void )
{
   int rc = sqlite3_exec( m_db, "ROLLBACK TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite rollback transaction", rc );
}

//-----------------------------------------------------------------------------

void database::savepoint( const std::string & name )
{
   std::string command( "SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite create savepoint " + name + " failed", rc );
}

//-----------------------------------------------------------------------------

void database::release_savepoint( const std::string & name )
{
   std::string command( "RELEASE SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
       throw_error( "SQLite release savepoint " + name + " failed", rc );
}

//-----------------------------------------------------------------------------

void database::rollback_to_savepoint( const std::string & name )
{
   std::string command( "ROLLBACK TO SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite rollback to savepoint " + name + " failed", rc );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
