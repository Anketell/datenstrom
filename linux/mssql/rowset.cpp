//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/rowset.h>
#include <mssql/error.h>
#include <sqlext.h>
#include <algorithm>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   m_stmt->prepare_result_desc();
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

template< typename T > void rowset::get_column( int index, int c_type, T & t )
{
   check_column( index );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];

   SQLLEN ind = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, c_type, &t, sizeof( T ), &ind );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   if ( ind == SQL_NULL_DATA )
      throw null_value();     
}

//-----------------------------------------------------------------------------

void rowset::get_text_column( int index, std::string & t )
{
   check_column( index );

   SQLLEN count = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_CHAR, t.data(), 0, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   if ( count == SQL_NULL_DATA )
      throw null_value();

   if ( count == SQL_NO_TOTAL )
   {
      rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_UNICODE, t.data(), 0, &count );
      check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
   }

   t.resize( count + 1 );

   rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_CHAR, t.data(), count + 1, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   t.resize( std::max( 0LL, static_cast< long long >( count ) ) );
}

//-----------------------------------------------------------------------------

void rowset::get_blob_column( int index, std::string & t )
{
   check_column( index );

   SQLLEN count = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_BINARY, t.data(), 0, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   if ( count == SQL_NULL_DATA )
      throw null_value();
      
   t.resize( count );

   rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_BINARY, t.data(), count, nullptr );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

void rowset::get_datetime_column( int index, std::string & t )
{
   check_column( index );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];

   SQLLEN count = desc.size;;

   t.resize( count + 1 );

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, SQL_C_CHAR, t.data(), count + 1, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   if ( count == SQL_NULL_DATA )
      throw null_value();
      
   std::string::reverse_iterator it;
   for ( it = t.rbegin(); it != t.rend(); it++ )
   {
      if ( *it != '\0' )
      {
         t.resize( t.length() - ( it - t.rbegin() ) );
         break;
      }
   }

   if ( t.find_last_of( '.' ) == std::string::npos )
      return;

   for ( it = t.rbegin(); *it == '0'; it++ )
      ;

   if ( *it == '.' )
      it++;

   count = t.length() - ( it - t.rbegin() );

   t.resize( std::max( 0LL, static_cast< long long >( count ) ) );
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

      case sql_datetime_type_1:
      case sql_datetime_type_2:
      case sql_datetime_type_3:
         get_datetime_column( index, s );
         break;

      default :
         get_text_column( index, s );
         break;
   }
}

//-----------------------------------------------------------------------------

bool rowset::get_column_null( int index )
{
   check_column( index );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];

   char ch[ 20 ];

   SQLSMALLINT type   = desc.type;
   size_t      length = sizeof( ch );

   switch ( type )
   {
      case SQL_BIGINT:
         if ( desc.size == 19 )
            type = SQL_C_SBIGINT;
         else
            type = SQL_C_UBIGINT;
         break;

      case SQL_FLOAT:
         type = SQL_C_DOUBLE;
         break;

      case SQL_BINARY :
      case SQL_VARBINARY :
      case SQL_LONGVARBINARY :
         type   = SQL_C_BINARY;
         length = 0;
         break;

      case sql_datetime_type_1:
      case sql_datetime_type_2:
      case sql_datetime_type_3:
      case SQL_VARCHAR:
         type = SQL_C_CHAR;
         break;
   }

   SQLLEN ind = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, type, &ch, length, &ind );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   return ind == SQL_NULL_DATA;
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
