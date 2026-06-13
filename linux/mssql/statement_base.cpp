//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/statement_base.h>
#include <mssql/rowset.h>
#include <mssql/error.h>
#include <dsutil/time.h>
#include <sqlext.h>
#include <cstring>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

template< typename T > T * statement_base::buffer::data( void )
{
   return reinterpret_cast< T * >( m_data );
}

//-----------------------------------------------------------------------------

void statement_base::buffer::resize( size_t size )
{
   if ( m_length && m_length < size )
   {
      free( m_data );
      m_length = 0;
   }

   if ( !m_length )
   {
      m_length = size;
      m_data = malloc( m_length );
   }
}

//-----------------------------------------------------------------------------

statement_base::buffer::~buffer( void )
{
   if ( m_length )
   {
      free( m_data );
      m_length = 0;
   }
}

//-----------------------------------------------------------------------------

statement_base::statement_base( SQLHDBC hdbc )
{
   static constexpr char operation[] = "MSSQL statement";

   m_stmt = std::make_shared< stmt_t >();

   RETCODE rc = SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &m_stmt->hstmt );
   check_status( operation, hdbc, SQL_HANDLE_DBC, rc );
}

//-----------------------------------------------------------------------------

void statement_base::prepare( const std::string& sql )
{
    static constexpr char operation[] = "MSSQL statement prepare";

    RETCODE rc = SQLPrepare( m_stmt->hstmt, sql_char( sql.c_str() ), sql_int( sql.length() ) );
    check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

    prepare_parameter_desc();
}

//-----------------------------------------------------------------------------

void statement_base::prepare_parameter_desc( void )
{
   static constexpr char operation[] = "MSSQL statement prepare parameter descriptions";

   SQLSMALLINT count;

   RETCODE rc = SQLNumParams( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_parameters.resize( count );
   m_buffers.resize( count );

   for ( SQLSMALLINT i = 0; i < count; i++ )
   {
      stmt_t::desc_t & desc( m_parameters[ i ] );

      rc = SQLDescribeParam( m_stmt->hstmt, i + 1, &desc.type,
                                                   &desc.size,
                                                   &desc.digits,
                                                   &desc.nullable );

      check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
   }
}

//-----------------------------------------------------------------------------

statement_base::buffer & statement_base::check_parameter( int index )
{
   static constexpr char operation[] = "MSSQL statement parameter check";

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_parameters.size() )
      throw_error( operation, "Too many parameters" );

   return m_buffers[ index ];
}

//-----------------------------------------------------------------------------

void statement_base::bind_text_parameter( int index, const std::string & t )
{
   static constexpr char operation[] = "MSSQL statement text parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   SQLLEN size = std::min( t.length(), static_cast< size_t >( desc.size ) ) + 1;

   buffer.resize( size );

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  SQL_C_CHAR,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data< void >(),
                                  size,
                                  nullptr );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   strncpy( buffer.data< char >(), t.c_str(), size );
}

//-----------------------------------------------------------------------------

void statement_base::bind_blob_parameter( int index, const std::string & t )
{
   static constexpr char operation[] = "MSSQL statement BLOB parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   int size = std::min( t.length(), static_cast< size_t >( desc.size ) );

   buffer.resize( size );

   desc.ind_len = size;

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  SQL_C_BINARY,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data< void >(),
                                  size,
                                  &desc.ind_len );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   memcpy( buffer.data< char >(), t.data(), size );
}

//-----------------------------------------------------------------------------

void statement_base::bind_parameter( int index, const std::string & t )
{
   check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   switch ( desc.type )
   {
      case SQL_BINARY:
      case SQL_VARBINARY:
      case SQL_LONGVARBINARY:
         bind_blob_parameter( index, t );
         break;

      default :
         bind_text_parameter( index, t );
         break;
   }
}

//-----------------------------------------------------------------------------

template< typename T > void statement_base::bind_parameter( int index, int c_type, const T & t )
{
   static constexpr char operation[] = "MSSQL statement parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   int size = sizeof( T );

   buffer.resize( size );

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  c_type,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data< void >(),
                                  size,
                                  nullptr         );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   *buffer.data< T >() = t;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int8_t i )
{
   bind_parameter< int8_t >( index, SQL_C_STINYINT, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int16_t i )
{
   bind_parameter< int16_t >( index, SQL_C_SSHORT, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int32_t i )
{
   bind_parameter< int32_t >( index, SQL_C_SLONG, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int64_t i )
{
   bind_parameter< int64_t >( index, SQL_C_SBIGINT, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint8_t u )
{
   bind_parameter< uint8_t >( index, SQL_C_UTINYINT, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint16_t u )
{
   bind_parameter< uint16_t >( index, SQL_C_USHORT, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint32_t u )
{
   bind_parameter< uint32_t >( index, SQL_C_ULONG, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint64_t u )
{
   bind_parameter< uint64_t >( index, SQL_C_UBIGINT, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, double d )
{
   bind_parameter< double >( index, SQL_C_DOUBLE, d );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const std::string & s )
{
   bind_parameter( index, s );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s )
{
   bind_text_parameter( index, s );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter_null( int index )
{
   static constexpr char operation[] = "MSSQL statement text parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   desc.ind_len = SQL_NULL_DATA;

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  SQL_C_DEFAULT,
                                  desc.type,
                                  0,
                                  0,
                                  nullptr,
                                  0,
                                  &desc.ind_len );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
}

//-----------------------------------------------------------------------------

int statement_base::parameter_count( void )
{
   return static_cast< int >( m_parameters.size() );
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

void statement_base::execute( void )
{
   m_stmt->execute();
   reset();
}

//-----------------------------------------------------------------------------

db::rowset statement_base::result( void )
{
   m_stmt->execute();

   return db::rowset( std::make_shared< mssql::rowset >( m_stmt ) );
}

//-----------------------------------------------------------------------------

}
