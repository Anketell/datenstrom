//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <postgresql/rowset.h>
#include <postgresql/error.h>
#include <dsutil/endian.h>
#include <dsutil/timestamp.h>
#include <libpq-fe.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------


rowset::rowset( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   m_row_count    = PQntuples( m_stmt->result );
   m_cursor       = 0;

   m_valid = m_row_count;

   configure_buffer();
}

//-----------------------------------------------------------------------------

void rowset::configure_buffer( void )
{
   if ( !m_valid )
      return;

   m_column_count = PQnfields( m_stmt->result );
   m_column_types.reserve( m_column_count );

   for ( int i = 0; i < m_column_count; i++ )
      m_column_types.push_back( PQftype( m_stmt->result, i ) );
}

//-----------------------------------------------------------------------------

rowset::~rowset( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return m_column_count;
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   char * tuples = PQcmdTuples( m_stmt->result );
   return std::atoi( tuples );
}

//-----------------------------------------------------------------------------

void rowset::check_column( int index )
{
   static constexpr char operation[] = "PostgreSQL rowset get column";

   if ( !m_valid )
      throw_error( operation, "No row available" );

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   if ( index >= m_column_count )
      throw_error( operation, "No column available" );
}

//-----------------------------------------------------------------------------

static constexpr char get_int_op[]    = "PostgreSQL get integer";
static constexpr char numeric_range[] = "Out of numeric range";

//-----------------------------------------------------------------------------

int64_t rowset::get_int( int index )
{
   if ( get_column_null( index ) )
      throw null_value();

   const char * value = PQgetvalue( m_stmt->result, m_cursor, index );

   switch ( m_column_types[ index ] )
   {
      case PG_SMALLINT:
         return endian::btoh( *reinterpret_cast< const int16_t * >( value ) );

      case PG_INTEGER:
         return endian::btoh( *reinterpret_cast< const int32_t * >( value ) );

      case PG_BIGINT:
         return endian::btoh( *reinterpret_cast< const int64_t * >( value ) );
   }
   throw_error( get_int_op, "Not integer type" );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int8_t & i )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< int8_t >::lowest();
   int64_t max    = std::numeric_limits< int8_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   i = static_cast< int8_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int16_t & i )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< int16_t >::lowest();
   int64_t max    = std::numeric_limits< int16_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   i = static_cast< int16_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int32_t & i )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< int32_t >::lowest();
   int64_t max    = std::numeric_limits< int32_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   i = static_cast< int32_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int64_t & i )
{
   i = get_int( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint8_t & u )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< uint8_t >::lowest();
   int64_t max    = std::numeric_limits< uint8_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   u = static_cast< uint8_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint16_t & u )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< uint16_t >::lowest();
   int64_t max    = std::numeric_limits< uint16_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   u = static_cast< uint16_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint32_t & u )
{
   int64_t i64 = get_int( index );

   int64_t lowest = std::numeric_limits< uint32_t >::lowest();
   int64_t max    = std::numeric_limits< uint32_t >::max();

   if ( i64 < lowest || i64 > max )
      throw_error( get_int_op, numeric_range );

   u = static_cast< uint32_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint64_t & u )
{
   int64_t i64 = get_int( index );

   if ( i64 < 0 )
      throw_error( get_int_op, numeric_range );

   u = static_cast< uint64_t >( i64 );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, double & d )
{
   if ( get_column_null( index ) )
      throw null_value();

   const char * value = PQgetvalue( m_stmt->result, m_cursor, index );

   switch ( m_column_types[ index ] )
   {
      case PG_SMALLINT:
         d = endian::btoh( *reinterpret_cast< const int16_t * >( value ) );
         break;

      case PG_INTEGER:
         d = endian::btoh( *reinterpret_cast< const int32_t * >( value ) );
         break;

      case PG_BIGINT:
         d =  endian::btoh( *reinterpret_cast< const int64_t * >( value ) );
         break;

      case PG_FLOAT:
      {
         uint32_t u = endian::btoh( *reinterpret_cast< const uint32_t * >( value ) );
         d = *reinterpret_cast< const float * >( &u );
         return;
      }

      case PG_DOUBLE:
      {
         uint64_t u = endian::btoh( *reinterpret_cast< const uint64_t * >( value ) );
         d = *reinterpret_cast< const double * >( &u );
         break;
      }

      default:
         throw_error( "PostgreSQL get double precision", "Not numeric type" );
   }
}

//-----------------------------------------------------------------------------

static const int64_t seconds_per_day   = 24 * 60 * 60;
static const int64_t epoc_to_2020_days = 10957;

//-----------------------------------------------------------------------------

static std::string decode_date( const char * value )
{
   struct tm tm;
   char      buffer[ time::hires_len + 1 ] = {};

   int64_t days = endian::btoh( *reinterpret_cast< const int32_t * >( value ) );

   time_t t = ( days + epoc_to_2020_days ) * seconds_per_day;

   time::gmtime( &t, &tm );

   time::format_iso_8601_date( &tm, buffer );

   return buffer;
}

//-----------------------------------------------------------------------------

static std::string decode_time( const char * value )
{
   time::stamp tm;
   char        buffer[ time::hires_time_len + 1 ] = {};

   int64_t us = endian::btoh( *reinterpret_cast< const int64_t * >( value ) );

   time_t t = us / 1000000;

   time::gmtime( &t, &tm );

   tm.nano_sec = ( us % 1000000 ) * 1000;

   time::format_iso_8601_time( &tm, buffer );

   return buffer;
}

//-----------------------------------------------------------------------------

static std::string decode_datetime( const char * value )
{
   time::stamp tm;
   char        buffer[ time::hires_datetime_len + 1 ] = {};

   int64_t us = endian::btoh( *reinterpret_cast< const int64_t * >( value ) );

   time_t t = us / 1000000 + epoc_to_2020_days * seconds_per_day;

   time::gmtime( &t, &tm );

   tm.nano_sec = ( us % 1000000 ) * 1000;

   time::format_iso_8601( &tm, buffer );

   return buffer;
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, std::string & s )
{
   if ( get_column_null( index ) )
      throw null_value();

   int length = PQgetlength( m_stmt->result, m_cursor, index ) ;

   const char * value = PQgetvalue( m_stmt->result, m_cursor, index );
   switch ( m_column_types[ index ] )
   {
      case PG_BLOB:
      case PG_TEXT:
         s.assign( value, PQgetlength( m_stmt->result, m_cursor, index ) );
         break;

      case PG_DATE:
         s = decode_date( value );
         break;

      case PG_TIME:
         s = decode_time( value );
         break;

      case PG_DATETIME:
         s = decode_datetime( value );
         break;

      default:
         throw_error( "PostgreSQL get text", "Not text type" );
   }
}

//-----------------------------------------------------------------------------

bool rowset::get_column_null( int index )
{
   check_column( index );
   return PQgetisnull( m_stmt->result, m_cursor, index );
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
{
   return m_valid = ++m_cursor < m_row_count;
}

//-----------------------------------------------------------------------------

bool rowset::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}
