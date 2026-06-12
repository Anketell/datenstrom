//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mysql/rowset.h>
#include <mysql/error.h>
#include <dsutil/timestamp.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   configure_buffer();

   if ( m_stmt->count )
      step();
   else
      m_valid = false;
}

//-----------------------------------------------------------------------------

void rowset::configure_buffer( void )
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
            break;

         case MYSQL_TYPE_SHORT:
            bind.buffer_length = sizeof( int16_t );
            break;

         case MYSQL_TYPE_INT24:
            bind.buffer_length = sizeof( int16_t ) + sizeof( int8_t );
            break;

         case MYSQL_TYPE_LONG:
            bind.buffer_length = sizeof( int32_t );
            break;

         case MYSQL_TYPE_LONGLONG:
            bind.buffer_length = sizeof( int64_t );
            break;

         case MYSQL_TYPE_FLOAT:
            bind.buffer_length = sizeof( float );
            break;

         case MYSQL_TYPE_DOUBLE:
            bind.buffer_length = sizeof( double );
            break;

         case MYSQL_TYPE_DATE:
         case MYSQL_TYPE_TIME:
         case MYSQL_TYPE_DATETIME:
            bind.buffer_length = 40;
            break;

         default:
            bind.buffer_length = field.length;
            break;
      }

      bind.buffer = malloc( bind.buffer_length );

      info.length  = bind.buffer_length;
      info.is_null = 0;
      info.error   = 0;
   }

   mysql_free_result( res );

   mysql_stmt_bind_result( m_stmt->stmt, m_stmt->mysql_bind );
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return m_stmt->count;
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   int rows = mysql_stmt_affected_rows( m_stmt->stmt );
   return rows > 0 ? rows : 0;
}

//-----------------------------------------------------------------------------

 void rowset::get_column( int              index,
                          enum_field_types type,
                          void           * p,
                          size_t           length,
                          int              is_unsigned )
 {
   static constexpr char operation[] = "MySQL rowset get column";

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   MYSQL_BIND  column = { 0 };
   bind_info_t info   = { 0 };

   column.buffer_type   = type;
   column.buffer_length = length;
   column.buffer        = p;
   column.is_unsigned   = is_unsigned;
   column.length        = &info.length;
   column.is_null       = &info.is_null;
   column.error         = &info.error;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );

   if ( info.is_null )
      throw null_value();

   switch ( rc )
   {
      case 0:
         break;

      case 1:
         throw_error( operation, "No row available" );

      default:
         throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
   }
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int8_t & i )
{
   get_column( index, MYSQL_TYPE_TINY, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int16_t & i )
{
   get_column( index, MYSQL_TYPE_SHORT, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int32_t & i )
{
   get_column( index, MYSQL_TYPE_LONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int64_t & i )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint8_t & u )
{
   get_column( index, MYSQL_TYPE_TINY, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint16_t & u )
{
   get_column( index, MYSQL_TYPE_SHORT, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint32_t & u )
{
   get_column( index, MYSQL_TYPE_LONG , &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint64_t & u )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, double & d )
{
   get_column( index, MYSQL_TYPE_DOUBLE, &d, sizeof( d ) );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, std::string & s )
{
   static constexpr char operation[] = "MySQL rowset get column";

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   MYSQL_BIND  column = { 0 };
   bind_info_t info   = { 0 };

   column.buffer_type   = MYSQL_TYPE_VAR_STRING;
   column.buffer_length = 0;
   column.length        = &info.length;
   column.is_null       = &info.is_null;
   column.error         = &info.error;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );

   if ( info.is_null )
      throw null_value();

   if ( rc == 0 )
   {
      s.resize( info.length );

      column.buffer_length = s.length();
      column.buffer        = const_cast< char * >( s.data() );

      rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   }

   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );

   switch ( m_stmt->mysql_bind[ index ].buffer_type )
   {
      case MYSQL_TYPE_TIME:
         ds::time::reformat_iso_8601_time( s );
         break;

      case MYSQL_TYPE_DATETIME:
         ds::time::reformat_iso_8601( s );
         break;
   }
}

//-----------------------------------------------------------------------------

bool rowset::get_column_null( int index )
{
   static constexpr char operation[] = "MySQL rowset get column null";

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   MYSQL_BIND  column = { 0 };
   bind_info_t info   = { 0 };

   column.buffer_type   = MYSQL_TYPE_VAR_STRING;
   column.buffer_length = 0;
   column.length        = &info.length;
   column.is_null       = &info.is_null;
   column.error         = &info.error;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );

   return info.is_null;
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
{
   m_valid = mysql_stmt_fetch( m_stmt->stmt ) == 0;

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool rowset::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}
