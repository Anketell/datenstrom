//-----------------------------------------------------------------------------

#include <mysql/positional_statement.h>
#include <mysql/row.h>
#include <mysql/error.h>
#include <sstream>
#include <string.h>
#include <assert.h>

#include <iostream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

positional_statement::positional_statement( MYSQL             & mysql,
                                        const std::string & sql ) :
statement_base( mysql, sql )
{
   prepare_parameter_binding();
}

//-----------------------------------------------------------------------------

positional_statement::~positional_statement( void )
{
   cleanup_parameters();
   delete m_mysql_bind;
}

//-----------------------------------------------------------------------------

void positional_statement::prepare_parameter_binding( void )
{
   m_bind_count = mysql_stmt_param_count( m_stmt->stmt );
   if ( m_bind_count )
   {
      m_mysql_bind = new MYSQL_BIND[ m_bind_count ];
      memset( m_mysql_bind, 0, sizeof( MYSQL_BIND ) * m_bind_count );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::cleanup_parameters( void )
{
   for ( int i = 0; i < m_bind_count; i++ )
      free( m_mysql_bind[ i ].buffer );
}

//-----------------------------------------------------------------------------

int positional_statement::check_parameter( int index )
{
   static constexpr char operation[] = "SQLite prepared statement parameter check";

   if ( m_state == Executed )
      reset();

   if ( index < 1 )
      throw_error( operation, "bad parameter" );

   if ( index > m_bind_count )
      throw_error( operation, "Too many parameters" );

   return index - 1;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int8_t i )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_TINY;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int16_t i )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_SHORT;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int32_t i )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_LONG;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy(  param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int64_t i )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_LONGLONG;
   param.buffer_length = sizeof( i );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &i, param.buffer_length );
   param.is_unsigned   = 0;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------


void positional_statement::set_parameter( int index, uint8_t u )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_TINY;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy(  param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint16_t u )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_SHORT;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint32_t u )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_LONG;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint64_t u )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_LONGLONG;
   param.buffer_length = sizeof( u );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &u, param.buffer_length );
   param.is_unsigned   = 1;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, double d )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_DOUBLE;
   param.buffer_length = sizeof( d );
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, &d, param.buffer_length );
   param.is_unsigned   = 0;
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const char * s, size_t length )
{
   MYSQL_BIND & param( m_mysql_bind[ check_parameter( index ) ] );

   param.buffer_type   = MYSQL_TYPE_STRING;
   param.buffer_length = length;
   param.buffer        = malloc( param.buffer_length );
   memcpy( param.buffer, s, param.buffer_length );
   param.is_null       = 0;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const char * s )
{
   set_parameter( index, s, strlen( s ) );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const std::string & s )
{
   set_parameter( index, s.data(), s.length() );
}

//-----------------------------------------------------------------------------

int positional_statement::parameter_count( void )
{
   return m_bind_count;
}

//-----------------------------------------------------------------------------

void positional_statement::reset( void )
{
   statement_base::reset();

   cleanup_parameters();
   memset( m_mysql_bind, 0, sizeof( MYSQL_BIND ) * m_bind_count );
}

//-----------------------------------------------------------------------------

void positional_statement::internal_execute( void )
{
   static constexpr char operation[] = "MySQL poitional statement parameter binding";

   if ( m_bind_count )
   {
      int rc = mysql_stmt_bind_param( m_stmt->stmt, m_mysql_bind );
      if ( rc )
         throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
   }

   statement_base::internal_execute();
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}