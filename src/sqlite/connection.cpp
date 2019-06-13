//-----------------------------------------------------------------------------

#include <sqlite/connection.h>
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

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

connection::connection( const std::string & path ) :
m_path( path )
{
}

//-----------------------------------------------------------------------------

connection::connection( db::connect_params_t params )
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
   static constexpr char operation[] = "SQLite create connection";

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
      throw_error( "SQLite use connection", rc );
   }
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   close();

   if ( unlink( get_full_path( name ).c_str() ) != 0 )
   {
      if ( errno == ENOENT )
         return false;

      throw_error( "SQLite drop connection", strerror( errno ) );
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
      throw_error( "SQLite create savepoint " + name + " failed", rc );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   std::string command( "RELEASE SAVEPOINT " );
   int rc = sqlite3_exec( m_db, ( command + name ).c_str(), NULL, NULL, NULL );
   if ( rc != SQLITE_OK )
       throw_error( "SQLite release savepoint " + name + " failed", rc );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
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
