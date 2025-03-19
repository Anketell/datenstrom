//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mysql/statement_base.h>
#include <mysql/rowset.h>
#include <mysql/error.h>
#include <db/simple_rowset.h>
#include <cstring>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

statement_base::statement_base( MYSQL & mysql )
{
   m_stmt = std::make_shared< stmt_t >();

   m_stmt->stmt = mysql_stmt_init( &mysql );
}

//-----------------------------------------------------------------------------

statement_base::~statement_base( void )
{
   cleanup_parameters();
   delete [] m_mysql_bind;
   delete [] m_bind_info;
}

//-----------------------------------------------------------------------------

void statement_base::prepare( const std::string & sql )
{
   int rc = mysql_stmt_prepare( m_stmt->stmt, sql.data(), sql.length() );
   if ( rc )
   {
      throw_error( "MySQL statement preparation",
                   mysql_stmt_error( m_stmt->stmt ) );
   }

   m_state = Preparing;

   prepare_parameter_binding();
}

//-----------------------------------------------------------------------------

void statement_base::internal_execute( void )
{
   static constexpr char operation[] = "MySQL statement execute";

   if ( m_bind_count )
   {
      int rc = mysql_stmt_bind_param( m_stmt->stmt, m_mysql_bind );
      if ( rc )
         throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
   }

   int rc = mysql_stmt_execute( m_stmt->stmt );
   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );

   m_state = Executed;
}

//-----------------------------------------------------------------------------

void statement_base::prepare_parameter_binding( void )
{
   m_bind_count = mysql_stmt_param_count( m_stmt->stmt );
   if ( m_bind_count )
   {
      m_mysql_bind = new MYSQL_BIND[ m_bind_count ];
      memset( m_mysql_bind, 0, sizeof( MYSQL_BIND ) * m_bind_count );

      m_bind_info = new bind_info_t[ m_bind_count ];
      memset( m_bind_info, 0, sizeof( bind_info_t ) * m_bind_count );

      for ( int i = 0; i < m_bind_count; i++ )
      {
         MYSQL_BIND & param( m_mysql_bind[ i ] );

         param.length  = &m_bind_info[ i ].length;
         param.is_null = &m_bind_info[ i ].is_null;
         param.error   = &m_bind_info[ i ].error;
      }
   }
}

//-----------------------------------------------------------------------------

void statement_base::cleanup_parameters( void )
{
   for ( int i = 0; i < m_bind_count; i++ )
   {
      MYSQL_BIND & param( m_mysql_bind[ i ] );

      if ( param.buffer )
      {
         free( param.buffer );
         param.buffer = nullptr;
      }
   }
}

//-----------------------------------------------------------------------------
/**/
void statement_base::check_parameter( int index )
{
   static constexpr char operation[] = "MySQL statement parameter check";

   if ( m_state == Executed )
      reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_bind_count )
      throw_error( operation, "Too many parameters" );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int8_t i )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_TINY;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int16_t i )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_SHORT;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int32_t i )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_LONG;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy(  param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int64_t i )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_LONGLONG;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint8_t u )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_TINY;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy(  param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint16_t u )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_SHORT;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint32_t u )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_LONG;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint64_t u )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_LONGLONG;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, double d )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_DOUBLE;
   param.buffer_length = sizeof( d );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &d, param.buffer_length );
   param.is_unsigned   = 0;

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s, size_t length )
{
   check_parameter( index );

   MYSQL_BIND & param( m_mysql_bind[ index ] );

   param.buffer_type   = MYSQL_TYPE_STRING;
   param.buffer_length = length;
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, s, param.buffer_length );

   bind_info_t & info( m_bind_info[ index ] );

   info.length  = param.buffer_length;
   info.is_null = 0;
   info.error   = 0;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s )
{
   set_parameter( index, s, strlen( s ) );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const std::string & s )
{
   set_parameter( index, s.data(), s.length() );
}

//-----------------------------------------------------------------------------

int statement_base::parameter_count( void )
{
   return m_bind_count;
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   int rc = mysql_stmt_reset( m_stmt->stmt );
   if ( rc )
      throw_error( "MySQL statement reset", mysql_stmt_error( m_stmt->stmt ) );

   cleanup_parameters();

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

void statement_base::execute( void )
{
   static constexpr char operation[] = "MySQL statement execute";

   internal_execute();
   reset();
}

//-----------------------------------------------------------------------------

db::rowset statement_base::result( void )
{
   m_stmt->stmt->mysql->insert_id = 0;
   
   internal_execute();

   db::rowset result( std::make_shared< mysql::rowset >( m_stmt ) );

   if ( result.eof() )
   {
      uint64_t value = mysql_stmt_insert_id( m_stmt->stmt );
      if ( value )
         result = db::rowset( std::make_shared< db::simple_rowset >( value ) );
   }

   return result;
}

//-----------------------------------------------------------------------------

}
