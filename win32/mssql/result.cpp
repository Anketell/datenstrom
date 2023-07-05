//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/result.h>
#include <mssql/error.h>
#include <dsutil/time.h>
#include <sqlext.h>
#include <algorithm>

#undef max
#undef min

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   step();
}

//-----------------------------------------------------------------------------

result::~result( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

int result::column_count( void ) const
{
   return static_cast< int >( m_stmt->columns.size() );
}

//-----------------------------------------------------------------------------

int result::rows_affected( void ) const
{
   static constexpr char operation[] = "MSSQL result rows affeected";

   SQLLEN count = 0;
   RETCODE rc = SQLRowCount( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   return static_cast< int >( std::abs( count ) );
}

//-----------------------------------------------------------------------------

void result::check_column( int index )
{
   static constexpr char operation[] = "MSSQL result column check";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   if ( index >= column_count() )
      throw_error( operation, "No column available" );

}

//-----------------------------------------------------------------------------

time_t result::get_time( int index )
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

static constexpr char operation[] = "MSSQL result get column";

//-----------------------------------------------------------------------------

template< typename T > void result::get_column( int index, int c_type, T & t )
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

template<> void result::get_column< std::string >( int index, int c_type, std::string & t )
{
   check_column( index );

   SQLLEN count = 0;

   RETCODE rc = SQLGetData( m_stmt->hstmt, index + 1, c_type, t.data(), 0, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   t.resize( count + 1 );

   rc = SQLGetData( m_stmt->hstmt, index + 1, c_type, t.data(), count + 1, nullptr );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   stmt_t::desc_t & desc = m_stmt->columns[ index ];
   if ( desc.type == sql_time_type )
   {
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
   }

   t.resize( std::max( 0LL, static_cast< int64_t >( count ) ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int8_t & i )
{
   get_column< int8_t >( index, SQL_C_STINYINT, i );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int16_t & i )
{
   get_column< int16_t >( index, SQL_C_SSHORT, i );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int32_t & i )
{
   get_column< int32_t >( index, SQL_C_SLONG, i );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int64_t & i )
{
   get_column< int64_t >( index, SQL_C_SBIGINT, i );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint8_t & u )
{
   get_column< uint8_t >( index, SQL_C_UTINYINT, u );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint16_t & u )
{
   get_column< uint16_t >( index, SQL_C_USHORT, u );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint32_t & u )
{
   get_column< uint32_t >( index, SQL_C_ULONG, u );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint64_t & u )
{
   get_column< uint64_t >( index, SQL_C_UBIGINT, u );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, double & d )
{
   get_column< double >( index, SQL_C_DOUBLE, d );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, std::string & s )
{
   get_column< std::string >( index, SQL_C_CHAR, s );
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   m_valid = SQLFetch( m_stmt->hstmt ) == SQL_SUCCESS;

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool result::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
