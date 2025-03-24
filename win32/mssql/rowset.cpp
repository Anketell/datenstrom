//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/rowset.h>
#include <mssql/error.h>
#include <dsutil/timestamp.h>
#include <sqlext.h>

#undef max
#undef min

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   step();
}

//-----------------------------------------------------------------------------

rowset::~rowset( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return static_cast< int >( m_stmt->columns.size() );
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   static constexpr char operation[] = "MSSQL rowset rows affeected";

   SQLLEN count = 0;
   RETCODE rc = SQLRowCount( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   return static_cast< int >( std::abs( count ) );
}

//-----------------------------------------------------------------------------

static constexpr char operation[] = "MSSQL rowset get column";

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

time_t rowset::get_time( int index )
{
   std::string time;

   get_column( index, time );

   struct tm tm;

   switch ( time.length() )
   {
      case time::time_len:
         time::parse_iso_8601_time( time.c_str(), &tm );
         tm.tm_year = 70;
         tm.tm_mday = 1;
         break;

      case time::date_len:
         time::parse_iso_8601_date( time.c_str(), &tm );
         break;

      case time::datetime_len:
         time::parse_iso_8601( time.c_str(), &tm );
         break;

      default:
         return 0;
   }

   return time::timegm( &tm );
}

//-----------------------------------------------------------------------------

template< typename T > void rowset::get_column( int index, int c_type, T & t )
{
   check_column( index );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];
   if ( desc.type == sql_time_type )
   {
      t = static_cast< T >( get_time( index ) );
      return;
   }

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, c_type, &t, sizeof( T ), nullptr  );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

void rowset::get_text_column( int index, std::string & t )
{
   check_column( index );

   SQLLEN count = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_CHAR, t.data(), 0, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   t.resize( count + 1 );

   rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_CHAR, t.data(), count + 1, nullptr );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];
   if ( desc.type == sql_time_type )
   {
      switch ( desc.size )
      {
         case 16:
            ds::time::reformat_iso_8601_time( t );
            break;

         case 27:
            ds::time::reformat_iso_8601( t );
            break;
      }
   }

   t.resize( std::max( 0LL, static_cast< int64_t >( count ) ) );
}

//-----------------------------------------------------------------------------

void rowset::get_blob_column( int index, std::string & t )
{
   check_column( index );

   SQLLEN count = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_BINARY, t.data(), 0, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   t.resize( count );

   rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_BINARY, t.data(), count, nullptr );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int8_t & i )
{
   get_column< int8_t >( index, SQL_C_STINYINT, i );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int16_t & i )
{
   get_column< int16_t >( index, SQL_C_SSHORT, i );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int32_t & i )
{
   get_column< int32_t >( index, SQL_C_SLONG, i );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int64_t & i )
{
   get_column< int64_t >( index, SQL_C_SBIGINT, i );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint8_t & u )
{
   get_column< uint8_t >( index, SQL_C_UTINYINT, u );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint16_t & u )
{
   get_column< uint16_t >( index, SQL_C_USHORT, u );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint32_t & u )
{
   get_column< uint32_t >( index, SQL_C_ULONG, u );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint64_t & u )
{
   get_column< uint64_t >( index, SQL_C_UBIGINT, u );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, double & d )
{
   get_column< double >( index, SQL_C_DOUBLE, d );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, std::string & s )
{
   check_column( index );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];

   switch ( desc.type )
   {
      case SQL_BINARY :
      case SQL_VARBINARY :
      case SQL_LONGVARBINARY :
         get_blob_column( index, s );
         break;

      default :
         get_text_column( index, s );
         break;
   }
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
{
   m_valid = SQLFetch( m_stmt->hstmt ) == SQL_SUCCESS;

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool rowset::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}
