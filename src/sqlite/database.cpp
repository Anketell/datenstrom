//-----------------------------------------------------------------------------

#include <sqlite/database.h>
#include <sqlite/statement.h>
#include <sqlite/error.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

database::database( const std::string & filename )
{
   int rc = sqlite3_open( filename.c_str(), &m_db );
   if ( rc )
   {
      sqlite3_close( m_db );
      throw_error( "sqlite3_open failed", rc );
   }
}

//-----------------------------------------------------------------------------

database::~database( void )
{
   sqlite3_close( m_db );
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
      throw_error( "BEGIN TRANSACTION failed", rc );
}

//-----------------------------------------------------------------------------

void database::commit_transaction( void )
{
   int rc = sqlite3_exec( m_db, "COMMIT TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "COMMIT TRANSACTION failed", rc );
}

//-----------------------------------------------------------------------------

void database::rollback_transaction( void )
{
   int rc = sqlite3_exec( m_db, "ROLLBACK TRANSACTION", NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( "ROLLBACK TRANSACTION failed", rc );
}

//-----------------------------------------------------------------------------

void database::savepoint( const std::string & name )
{
   std::string command( "SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( command + " failed", rc );
}

//-----------------------------------------------------------------------------

void database::release_savepoint( const std::string & name )
{
   std::string command( "RELEASE SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
       throw_error( command + " failed", rc );
}

//-----------------------------------------------------------------------------

void database::rollback_to_savepoint( const std::string & name )
{
   std::string command( "ROLLBACK TO SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
      throw_error( command + " failed", rc );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
