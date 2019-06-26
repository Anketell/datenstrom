//-----------------------------------------------------------------------------

#include <mysql/positional_statement.h>
#include <mysql/result.h>
#include <mysql/error.h>

#include <sstream>
#include <iostream>

#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

static my_bool not_null = 0;

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
   delete [] m_mysql_bind;
   delete [] m_bind_info;
}

//-----------------------------------------------------------------------------

void positional_statement::prepare_parameter_binding( void )
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
         m_mysql_bind[ i ].length  = &m_bind_info[ i ].length;
         m_mysql_bind[ i ].is_null = &m_bind_info[ i ].is_null;
         m_mysql_bind[ i ].error   = &m_bind_info[ i ].error;
      }
   }
}

//-----------------------------------------------------------------------------

void positional_statement::cleanup_parameters( void )
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

int positional_statement::check_parameter( int index )
{
   static constexpr char operation[] = "MySQL positional statement parameter check";

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
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, int16_t i )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, int32_t i )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, int64_t i )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, uint8_t u )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, uint16_t u )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, uint32_t u )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, uint64_t u )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, double d )
{
   index = check_parameter( index );

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

void positional_statement::set_parameter( int index, const char * s, size_t length )
{
   index = check_parameter( index );

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