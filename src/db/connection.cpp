//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/connection.h>
#include <db/enroll.h>
#include <dsutil/connect_string.h>
#include <dsutil/env.h>

#include <cstring>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

factory connection::m_factory;
bool    connection::m_initialized = false;

//-----------------------------------------------------------------------------

void connection::init( void )
{
   if ( !m_initialized )
      enroll_db_path_list( env::get( "DS_MODULE_PATH" ) );
}

//-----------------------------------------------------------------------------

void connection::enroll_db_path_list( const std::string & path_list )
{
   for ( auto path : env::dir_list( path_list ) )
      enroll_directory( m_factory, path );

   m_initialized = true;
}

//-----------------------------------------------------------------------------

void connection::clear_db_path_list( void )
{
   m_factory.deregister_all();
}

//-----------------------------------------------------------------------------

connection::connection( const std::string & connect_string ) :
connection( parse_connect_string( connect_string ) )
{
}

//-----------------------------------------------------------------------------

connection::connection( const ds::connect_params_t & connect_params )
{
   init();
   m_impl.reset( m_factory( connect_params ) );
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return m_impl->type();
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   m_impl->create( name );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   m_impl->use( name );
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   return m_impl->drop( name );
}

//-----------------------------------------------------------------------------

statement connection::operator()( const std::string & query,
                                  const name_list_t & parameters )
{
   return ( *m_impl )( query, parameters );
}

//-----------------------------------------------------------------------------

void connection::execute_batch( const std::string & query )
{
   m_impl->execute_batch( query );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   if ( !in_transaction() )
      m_impl->begin_transaction();

   transactional::begin_transaction();
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   transactional::commit_transaction();
   if ( !in_transaction() )
      m_impl->commit_transaction();
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   transactional::rollback_transaction();
   if ( !in_transaction() )
      m_impl->rollback_transaction();
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   m_impl->savepoint( name );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   m_impl->release_savepoint( name );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   m_impl->rollback_to_savepoint( name );
}

//-----------------------------------------------------------------------------

}
