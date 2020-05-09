//-----------------------------------------------------------------------------

#include <mysql/result.h>
#include <mysql/error.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   configure_buffer();
/*
   if ( m_stmt->count )
      step();
   else
      m_valid = false;
*/
   if ( step() )
      m_stmt->count = mysql_stmt_field_count( m_stmt->stmt );

}

//-----------------------------------------------------------------------------

void result::configure_buffer( void )
{
   if ( m_stmt->mysql_bind )
      return;

   MYSQL_RES * res = mysql_stmt_result_metadata( m_stmt->stmt );

   if ( !res )
      return;

   MYSQL_FIELD * fields = mysql_fetch_fields( res );

   m_stmt->count      = mysql_num_fields( res );
   m_stmt->mysql_bind = new MYSQL_BIND[ m_stmt->count ];
   m_stmt->bind_info  = new bind_info_t[ m_stmt->count ];

   for ( int i = 0; i < m_stmt->count; i++ )
   {
      MYSQL_BIND  & bind( m_stmt->mysql_bind[ i ] );
      bind_info_t & info( m_stmt->bind_info[ i ] );
      MYSQL_FIELD & field( fields[ i ] );

      bind.length      = &info.length;
      bind.is_null     = &info.is_null;
      bind.error       = &info.error;
      bind.is_unsigned = false;

      bind.buffer_type = field.type;

      switch ( bind.buffer_type )
      {
         case MYSQL_TYPE_TINY:
            bind.buffer_length = sizeof( int8_t );
            bind.buffer        = new int64_t;
            break;

         case MYSQL_TYPE_SHORT:
            bind.buffer_length = sizeof( int16_t );
            bind.buffer        = new int64_t;
            break;

         case MYSQL_TYPE_INT24:
            bind.buffer_length = sizeof( int16_t ) + sizeof( int8_t );
            bind.buffer        = new int64_t;
            break;

         case MYSQL_TYPE_LONG:
            bind.buffer_length = sizeof( int32_t );
            bind.buffer        = new int64_t;
            break;

         case MYSQL_TYPE_LONGLONG:
            bind.buffer_length = sizeof( int64_t );
            bind.buffer        = new int64_t;
            break;

         case MYSQL_TYPE_FLOAT:
            bind.buffer_length = sizeof( float );
            bind.buffer        = new double;
            break;

         case MYSQL_TYPE_DOUBLE:
            bind.buffer_length = sizeof( double );
            bind.buffer        = new double;
            break;

         default:
            bind.buffer_type   = field.type;
            bind.buffer_length = field.length;
            bind.buffer        = new char[ bind.buffer_length ];
            break;
      }

      info.length  = bind.buffer_length;
      info.is_null = false;
      info.error   = false;
   }

   mysql_free_result( res );

   mysql_stmt_bind_result( m_stmt->stmt, m_stmt->mysql_bind );
}

//-----------------------------------------------------------------------------

int result::column_count( void ) const
{
   return m_stmt->count;
}

//-----------------------------------------------------------------------------

int result::rows_affected( void ) const
{
   int rows = mysql_stmt_affected_rows( m_stmt->stmt );
   return rows > 0 ? rows : 0;
}

//-----------------------------------------------------------------------------

 void result::get_column( int              index,
                          enum_field_types type,
                          void           * p,
                          size_t           length,
                          int              is_unsigned )
 {
   static constexpr char operation[] = "MySQL get result column";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   MYSQL_BIND  column = {};
   bind_info_t info   = { length, 0, 0 };

   column.buffer_type   = type;
   column.buffer_length = length;
   column.buffer        = p;
   column.is_unsigned   = is_unsigned;
   column.length        = &info.length;
   column.is_null       = &info.is_null;
   column.error         = &info.error;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int8_t & i )
{
   get_column( index, MYSQL_TYPE_TINY, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int16_t & i )
{
   get_column( index, MYSQL_TYPE_SHORT, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int32_t & i )
{
   get_column( index, MYSQL_TYPE_LONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int64_t & i )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint8_t & u )
{
   get_column( index, MYSQL_TYPE_TINY, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint16_t & u )
{
   get_column( index, MYSQL_TYPE_SHORT, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint32_t & u )
{
   get_column( index, MYSQL_TYPE_LONG , &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint64_t & u )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, double & d )
{
   get_column( index, MYSQL_TYPE_DOUBLE, &d, sizeof( d ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, std::string & s )
{
   static constexpr char operation[] = "MySQL get result column";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   unsigned long length = 0;

   MYSQL_BIND column = { 0 };

   column.buffer_type   = MYSQL_TYPE_VAR_STRING;
   column.buffer_length = 0;
   column.length        = &length;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   if ( rc == 0 )
   {
      s.resize( length );

      column.buffer_length = s.length();
      column.buffer        = const_cast< char * >( s.data() );

      rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   }

   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   m_valid = mysql_stmt_fetch( m_stmt->stmt ) == 0;

   return *this;
}

//-----------------------------------------------------------------------------

result::operator bool ( void ) const
{
   return m_valid;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
