//-----------------------------------------------------------------------------

#include <mssql/statement_base.h>
// #include <mssql/result.h>
#include <mssql/error.h>

#include <sqlext.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

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

    prepare_parameter_buffer();
//    prepare_result_buffer();
}

//-----------------------------------------------------------------------------

void statement_base::prepare_parameter_buffer( void )
{
   static constexpr char operation[] = "MSSQL statement prepare parameter buffer";

   SQLSMALLINT count;

   RETCODE rc = SQLNumParams( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_parameters.resize( count );
   m_buffers.resize( count, { 0, nullptr } );

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

void statement_base::prepare_result_buffer(void)
{
   static constexpr char operation[] = "MSSQL statement prepare result buffer";

   SQLSMALLINT count;

   RETCODE rc = SQLNumResultCols( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_parameters.resize( count );

   for ( SQLSMALLINT i = 0; i < count; i++ )
   {
      stmt_t::desc_t & desc( m_stmt->columns[ i ] );

      rc = SQLDescribeCol( m_stmt->hstmt, i + 1, nullptr, 
                                                 0,
                                                 nullptr, 
                                                 &desc.type, 
                                                 &desc.size, 
                                                 &desc.digits, 
                                                 &desc.nullable );

      check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );
   }
}

//-----------------------------------------------------------------------------

statement_base::buffer_t & statement_base::check_parameter( int index, int size )
{
   static constexpr char operation[] = "MSSQL statement parameter check";

   reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_parameters.size() )
      throw_error( operation, "Too many parameters" );

   buffer_t & buffer = m_buffers[ index ];

   if ( buffer.length && buffer.length < size )
   {
      free( buffer.data );
      buffer.length = 0;
   }

   if ( !buffer.length )
   {
      buffer.length = size;
      buffer.data   = malloc( buffer.length );
   }

   return buffer;
}

//-----------------------------------------------------------------------------

template< typename T > void statement_base::bind_parameter( int index, int c_type, const T & t )
{
   static constexpr char operation[] = "MSSQL statement parameter bind";

   int size = sizeof( T );

   buffer_t & buffer = check_parameter( index, size );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT, 
                                  c_type,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data,
                                  size,
                                  nullptr         );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   *reinterpret_cast< T * >( buffer.data ) = t;
}

//-----------------------------------------------------------------------------

template<> void statement_base::bind_parameter< std::string >( int index, int c_type, const std::string & t )
{
   static constexpr char operation[] = "MSSQL statement parameter bind";

   stmt_t::desc_t & desc( m_parameters[ index ] );

   int size = min( t.length(), desc.size ) + 1;

   buffer_t & buffer = check_parameter( index, size );

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  c_type,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data,
                                  size,
                                  nullptr );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   memcpy( buffer.data, t.c_str(), size );
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
   bind_parameter< std::string >( index, SQL_C_CHAR, s );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s )
{
   bind_parameter< std::string >( index, SQL_C_CHAR, s );
}

//-----------------------------------------------------------------------------

int statement_base::parameter_count( void )
{
   return static_cast< int >( m_parameters.size() );
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   if ( m_state == Preparing )
      return;

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

uint64_t statement_base::execute( void )
{
   static constexpr char operation[] = "MSSQL statement execute";

   RETCODE rc = SQLExecute( m_stmt->hstmt );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   uint64_t res = 0;

   reset();
 
   return res;
}

//-----------------------------------------------------------------------------

db::result statement_base::result( void )
{
   reset();

//   db::result result = db::result( std::make_shared< sqlite::result >( m_stmt ) );

   m_state = Executed;

   return db::result();
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
