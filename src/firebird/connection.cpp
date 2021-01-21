//-----------------------------------------------------------------------------

#include <firebird/connection.h>
#include <firebird/positional_statement.h>
#include <firebird/named_statement.h>
#include <firebird/statement_enum.h>
#include <firebird/types.h>
#include <firebird/error.h>
#include <firebird/guard.h>

#include <db/transaction.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

constexpr char connection::TYPE[];

//-----------------------------------------------------------------------------

connection::connection( const std::string & server,
                        const std::string & path,
                        const std::string & username,
                        const std::string & password,
                        const std::string & ext,
                        uint16_t            port,
                        int                 dialect ) :
m_server( server ),
m_path( path ),
m_ext( ext ),
m_port( port ),
m_dialect( dialect )
{
   construct_dpb( username, password, m_dialect );
}

//-----------------------------------------------------------------------------

void connection::construct_dpb( const std::string & username,
                                const std::string & password,
                                int                 dialect )
{
   m_dpb.erase();

   m_dpb += isc_dpb_version1;
   m_dpb += isc_dpb_user_name;

   m_username_offset = m_dpb.length();

   m_dpb += static_cast< int8_t >( username.length() );
   m_dpb += username;
   m_dpb += isc_dpb_password;

   m_password_offset = m_dpb.length();

   m_dpb += static_cast< int8_t >( password.length() );
   m_dpb += password;

   m_dpb += isc_dpb_sql_dialect;
   m_dpb += 1;
   m_dpb += dialect & 0xff;
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

std::string connection::get_local_path( const std::string & name ) const
{
   return m_path + "/" + name + "." + m_ext;
}

//-----------------------------------------------------------------------------

std::string connection::get_full_path( const std::string & name ) const
{
   return m_server + ":" + get_local_path( name );
}

//-----------------------------------------------------------------------------

void connection::detach( void )
{
   if (  m_transactional.db_handle )
   {
      ISC_STATUS status[ status_vector_length ];

      isc_detach_database( status, &m_transactional.db_handle );

      check_status( "Firebird detach database", status );

      m_attached_db.erase();
   }
}

//-----------------------------------------------------------------------------

void connection::set_sql_dialect( int dialect )
{
   ISC_STATUS status[ status_vector_length ];

   isc_svc_handle svc_handle = 0;

   std::string spb;

   spb += isc_spb_version;
   spb += isc_spb_current_version;
   spb += isc_spb_user_name;
   spb += m_dpb.substr( m_username_offset, static_cast< size_t >( m_dpb[ m_username_offset ] ) + 1 );
   spb += isc_spb_password;
   spb += m_dpb.substr( m_password_offset, static_cast< size_t >( m_dpb[ m_password_offset ] ) + 1 );

   isc_service_attach( status, 0,
                       ( m_server + ":service_mgr" ).c_str(),
                       &svc_handle,
                       static_cast< int16_t >( spb.length() ), spb.data() );

   check_status( "Firebird create database", status );

   std::string local_path = get_local_path( m_attached_db );

   std::string request;

   request += isc_action_svc_properties;
   request += isc_spb_dbname;
   request += local_path.length() & 0xff;
   request += static_cast< int8_t >( local_path.length() >> 8 );
   request += local_path;
   request += isc_spb_prp_set_sql_dialect;
   request += dialect & 0xff;
   request += ( dialect >> 8 ) & 0xff;
   request += ( dialect >> 16 ) & 0xff;
   request += ( dialect >> 24 ) & 0xff;

   isc_service_start( status,
                      &svc_handle,
                      nullptr,
                      request.length(),
                      request.data() );

   isc_service_detach( status, &svc_handle );
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   detach();

   ISC_STATUS status[ status_vector_length ];

   std::string full_path = get_full_path( name );

   isc_create_database( status,
                        full_path.length(),
                        full_path.c_str(),
                        &m_transactional.db_handle,
                        m_dpb.length(),
                        m_dpb.c_str(),
                        0 );

   check_status( "Firebird create database", status );

   m_attached_db = name;

   set_sql_dialect( m_dialect );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   ISC_STATUS status[ status_vector_length ];

   if ( m_transactional.db_handle )
   {
      if ( name == m_attached_db )
         return;

      detach();
   }

   if ( !name.empty() )
   {
      std::string full_path = get_full_path( name );

      isc_attach_database( status,
                           full_path.length(),
                           full_path.c_str(),
                           &m_transactional.db_handle,
                           m_dpb.length(),
                           m_dpb.c_str() );

      check_status( "Firebird use database", status );

      m_attached_db = name;
   }
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   try
   {
      use( name );
   }
   catch ( ... )
   {
      return false;
   }

   ISC_STATUS status[ status_vector_length ];

   isc_drop_database( status, &m_transactional.db_handle );

   check_status( "Firebird drop database", status );

   return true;
}

//-----------------------------------------------------------------------------

db::statement connection::operator()( const std::string     & query,
                                      const db::name_list_t & parameters )
{
   if ( parameters.empty() )
      return db::statement( std::make_shared< positional_statement >( m_transactional,
                                                                      query            ) );

   return db::statement( std::make_shared< named_statement >( m_transactional,
                                                              query,
                                                              parameters       ) );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   guard( m_transactional, [ & ]( void )
   {
      ISC_STATUS status[ status_vector_length ];

      for ( auto & statement : statement_enum( query ) )
      {
         isc_dsql_execute_immediate( status,
                                     &m_transactional.db_handle,
                                     &m_transactional.tr_handle,
                                     statement.len, statement.from,
                                     3, nullptr );

         check_status( "Firebird execute batch", status );
      }
   } );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   m_transactional.begin_transaction();
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   m_transactional.commit_transaction();
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   m_transactional.rollback_transaction();
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   ISC_STATUS status[ status_vector_length ];

   std::string statement = "SAVEPOINT " + name;

   isc_dsql_execute_immediate( status,
                               &m_transactional.db_handle,
                               &m_transactional.tr_handle,
                               statement.length(), statement.c_str(),
                               3, nullptr );

   check_status( "Firebird savepoint", status );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   ISC_STATUS status[ status_vector_length ];

   std::string statement = "RELEASE SAVEPOINT " + name;

   isc_dsql_execute_immediate( status,
                               &m_transactional.db_handle,
                               &m_transactional.tr_handle,
                               statement.length(), statement.c_str(),
                               3, nullptr );

   check_status( "Firebird release savepoint", status );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   ISC_STATUS status[ status_vector_length ];

   std::string statement = "ROLLBACK TO " + name;

   isc_dsql_execute_immediate( status,
                               &m_transactional.db_handle,
                               &m_transactional.tr_handle,
                               statement.length(), statement.c_str(),
                               3, nullptr );

   check_status( "Firebird rollback savepoint", status );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
