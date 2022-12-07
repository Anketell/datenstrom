//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/context.h>
#include <dsutil/filesys.h>
#include <dsutil/module.h>
#include <dsutil/env.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

sql::map_t context::m_sql_map;
bool       context::m_initialized = false;

//-----------------------------------------------------------------------------

void context::init( void )
{
   if ( !m_initialized )
      enroll_sql_path_list( env::get( "SQL_MODULE_PATH" ) );
}

//-----------------------------------------------------------------------------

void context::enroll_sql_path_list( const std::string & path_list )
{
   for ( auto path :  env::dir_list( path_list ) )
      sql::enroll_directory( m_sql_map, path );

   m_initialized = true;
}

//-----------------------------------------------------------------------------

void context::clean_up( void )
{
   m_sql_map.clear();
   m_initialized = false;
}

//-----------------------------------------------------------------------------

context::context( connection & con ) :
connection( con )
{
   common_constructor();
}

//-----------------------------------------------------------------------------

context::context( const std::string & connect_string ) :
connection( connect_string )
{
   common_constructor();
}

//-----------------------------------------------------------------------------

context::context( const ds::connect_params_t & connect_params ) :
connection( connect_params )
{
   common_constructor();
}

//-----------------------------------------------------------------------------

void context::common_constructor( void )
{
   init();

   auto it_begin = m_sql_map.lower_bound( type() );
   auto it_end   = m_sql_map.upper_bound( type() );

   for ( auto it = it_begin; it != it_end; it++ )
      m_sql_lookup.push_front( it->second );

   if ( m_sql_lookup.empty() )
      throw unsupported_db_type( type() );
}

//-----------------------------------------------------------------------------

statement context::operator()( const std::string & query_key,
                               const name_list_t & parameters )
{
   return connection::operator()( lookup( query_key ), parameters );
}

//-----------------------------------------------------------------------------

void context::execute_batch( const std::string & query_key )
{
   connection::execute_batch( lookup( query_key ) );
}

//-----------------------------------------------------------------------------

const char * context::lookup( const std::string & key )
{
   const char * v = nullptr;

   for ( auto & lu : m_sql_lookup )
   {
      v = ( *lu )( key );
      if ( v )
         break;
   }

   if ( !v )
      throw context::unknown_sql( type(), key );

   return v;
}

//-----------------------------------------------------------------------------

}