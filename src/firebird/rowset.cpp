//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/rowset.h>
#include <firebird/error.h>
#include <firebird/transactional.h>
#include <limits>
#include <type_traits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< stmt_t >              stmt,
                transactional                        & transactional,
                std::unique_ptr< ds::db::transaction > transaction ) :
m_stmt( stmt ),
m_transactional( transactional ),
m_transaction( std::move( transaction ) )
{
   static constexpr char operation[] = "Firebird rowset prepare";

   m_valid = m_stmt->xsqlda->sqld;

   if ( m_valid )
   {
      if ( m_stmt->type == isc_info_sql_stmt_exec_procedure )
         m_valid = true;
      else
         step();
   }
   else
      m_transaction.reset();
}

//-----------------------------------------------------------------------------

rowset::~rowset( void )
{
   if ( m_transaction )
   {
      m_transaction.reset();
      m_stmt->state = stmt_t::Preparing;
   }
   else
   {
      if ( m_stmt->type == isc_info_sql_stmt_insert )
      m_stmt->state = stmt_t::Preparing;
   }
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return m_stmt->xsqlda->sqld;
}

//-----------------------------------------------------------------------------

static int isc_info_mask( int32_t type )
{
   int mask;

   switch ( type )
   {
      case isc_info_sql_stmt_select:
      case isc_info_sql_stmt_insert:
      case isc_info_sql_stmt_update:
      case isc_info_sql_stmt_delete:
         mask = 1 << ( type + isc_info_req_select_count - isc_info_sql_stmt_select );
         break;

      default:
         mask = 0x0f << isc_info_req_select_count;
         break;
   }

   return mask;
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   static constexpr char operation[] = "Firebird rows affected";

   ISC_STATUS status[ status_vector_length ];

   char info[] = { isc_info_sql_records, isc_info_end };
   char res[ 128 ];

   isc_dsql_sql_info( status,
                      &m_stmt->stmt,
                      sizeof( info ), info,
                      sizeof( res ), res );

   check_status( operation, status );

   int count = 0;

   char * p = res;

    if ( *p == isc_info_sql_records )
   {
      int mask = isc_info_mask( m_stmt->type );

      p += 3;
      while ( *p != isc_info_end )
      {
         if ( mask & ( 1 << *p ) )
         {
            int16_t len = isc_vax_integer( p + 1, 2 );
            count      += isc_vax_integer( p + 3, len );
         }
         p += 7;
      }
   }

   return count;
}

//-----------------------------------------------------------------------------

static constexpr char operation[] = "Firebird rowset get column";

//-----------------------------------------------------------------------------

void rowset::check_column( int index )
{
   if ( !m_valid )
      throw_error( operation, "No row available" );

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   if ( index >= column_count() )
      throw_error( operation, "No column available" );
}

//-----------------------------------------------------------------------------

template< typename BI > BI rowset::get_big_int( int index )
{
   typedef std::numeric_limits< BI > limits;

   typedef typename std::conditional< limits::is_signed, int16_t, uint16_t >::type SI;
   typedef typename std::conditional< limits::is_signed, int32_t, uint32_t >::type LI;

   check_column( index );

   BI bi;

   XSQLVAR & column( m_stmt->xsqlda->sqlvar[ index ] );

   switch ( column.sqltype & ~1 )
   {
      case SQL_SHORT:
         bi = static_cast< BI >( *reinterpret_cast< SI * >( column.sqldata ) );
         break;

      case SQL_LONG:
         bi = static_cast< BI >( *reinterpret_cast< LI * >( column.sqldata ) );
         break;

      case SQL_INT64:
         bi = *reinterpret_cast< BI * >( column.sqldata );
         break;

      case SQL_TYPE_DATE:
         bi = decode_sql_unixdate( *reinterpret_cast< ISC_DATE * >( column.sqldata ) );
         break;

      case SQL_TYPE_TIME:
         bi = decode_sql_unixtime( *reinterpret_cast< ISC_TIME * >( column.sqldata ) );
         break;

      case SQL_TIMESTAMP:
         bi = decode_unixtimestamp( *reinterpret_cast< ISC_TIMESTAMP * >( column.sqldata ) );
         break;

      default:
         throw_error( operation, "Not integer type" );
   }

   return bi;
}

//-----------------------------------------------------------------------------

template< typename I > I rowset::get_integer( int index )
{
   typedef std::numeric_limits< I > limits;

   typedef typename std::conditional< limits::is_signed, int64_t, uint64_t >::type BI;

   BI bi = get_big_int< BI >( index );

   if ( bi < limits::lowest() || bi > limits::max() )
   {
      std::string message( "Integer data value beyond " );
      throw_error( operation, ( message + typeid( I ).name() ).c_str() );
   }

   return static_cast< I >( bi );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int8_t & i )
{
   i = get_integer< int8_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int16_t & i )
{
   i = get_integer< int16_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int32_t & i )
{
   i = get_integer< int32_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int64_t & i )
{
   i = get_big_int< int64_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint8_t & u )
{
   u = get_integer< uint8_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint16_t & u )
{
   u = get_integer< uint16_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint32_t & u )
{
   u = get_integer< uint32_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint64_t & u )
{
   u = get_big_int< uint64_t >( index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, double & d )
{
   check_column( index );

   XSQLVAR & column( m_stmt->xsqlda->sqlvar[ index ] );

   switch ( column.sqltype & ~1 )
   {
      case SQL_SHORT:
         d = static_cast< double >( *reinterpret_cast< int16_t * >( column.sqldata ) );
         break;

      case SQL_LONG:
         d = static_cast< double >( *reinterpret_cast< int32_t * >( column.sqldata ) );
         break;

      case SQL_INT64:
         d = static_cast< double >( *reinterpret_cast< int64_t * >( column.sqldata ) );
         break;

      case SQL_FLOAT:
         d = *reinterpret_cast< float * >( column.sqldata );
         break;

      case SQL_DOUBLE:
         d = *reinterpret_cast< double * >( column.sqldata );
         break;

      default:
         throw_error( operation, "Not numeric type" );
   }
}

//-----------------------------------------------------------------------------

static uint32_t get_blob_size( isc_blob_handle & bl_handle )
{
   static constexpr char operaton[] = "Firebird get blob info";

   static constexpr char request[] =
   {
      isc_info_blob_total_length
   };

   ISC_STATUS status[ status_vector_length ];

   char blob_info[ 32 ];

   isc_blob_info( status, &bl_handle,
                          sizeof( request ),
                          request,
                          sizeof( blob_info ),
                          blob_info );
   check_status( operation, status );

   int16_t len = isc_vax_integer( blob_info + 1, 2 );

   return isc_vax_integer( blob_info + 3, len );
}

//-----------------------------------------------------------------------------

void rowset::read_blob( int index, std::string & s )
{
   static constexpr char operation[] = "Firebird read blob";

   ISC_STATUS status[ status_vector_length ];

   isc_blob_handle bl_handle = 0;

   XSQLVAR & column( m_stmt->xsqlda->sqlvar[ index ] );

   isc_open_blob2( status, &m_transactional.db_handle,
                           &m_transactional.tr_handle,
                           &bl_handle,
                           reinterpret_cast< ISC_QUAD * >( column.sqldata ),
                           0,
                           nullptr );
   check_status( operation, status );

   uint32_t size = get_blob_size( bl_handle );

   s.resize( size );
   char * dest = const_cast< char * >( s.data() );

   while ( size )
   {
      uint16_t len = std::min( size, 65535U );
      uint16_t act_len;

      isc_get_segment( status, &bl_handle,
                               &act_len,
                               len,
                               reinterpret_cast< ISC_SCHAR * >( dest ) );
      check_status( operation, status );

      size -= act_len;
      dest += act_len;
   }

   isc_close_blob( status, &bl_handle );
   check_status( operation, status );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, std::string & s )
{
   check_column( index );

   XSQLVAR & column( m_stmt->xsqlda->sqlvar[ index ] );

   switch ( column.sqltype & ~1 )
   {
      case SQL_TEXT:
         s.assign( column.sqldata, column.sqllen );
         break;

      case SQL_VARYING:
         s.assign( column.sqldata + 2, *reinterpret_cast< uint16_t * >( column.sqldata ) );
         break;

      case SQL_TYPE_DATE:
         s = decode_sql_date( *reinterpret_cast< ISC_DATE * >( column.sqldata ) );
         break;

      case SQL_TYPE_TIME:
         s = decode_sql_time( *reinterpret_cast< ISC_TIME * >( column.sqldata ) );
         break;

      case SQL_TIMESTAMP:
         s = decode_timestamp( *reinterpret_cast< ISC_TIMESTAMP * >( column.sqldata ) );
         break;

      case SQL_BLOB:
         read_blob( index, s );
         break;

      default:
         throw_error( operation, "Not character type" );
   }
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
{
   ISC_STATUS status[ status_vector_length ];

   m_valid = !isc_dsql_fetch( status, &m_stmt->stmt, 1, m_stmt->xsqlda );

   if ( !m_valid && m_transaction )
   {
      m_stmt->state = stmt_t::Preparing;
      m_transaction.reset();
   }

   check_status( operation, status );

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool rowset::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
