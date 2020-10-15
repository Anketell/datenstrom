//-----------------------------------------------------------------------------

#include <mssql/statement_base.h>
#include <mssql/result.h>
#include <mssql/error.h>
#include <util/time.h>
#include <sqlext.h>

#undef min
#undef max

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

template< typename T > T * statement_base::buffer::data( void )
{
   return reinterpret_cast< T * >( m_data );
}

//-----------------------------------------------------------------------------

void statement_base::buffer::resize( int size )
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

void statement_base::prepare_result_desc( void )
{
   static constexpr char operation[] = "MSSQL statement prepare result descriptions";

   if ( m_stmt->columns.size() )
      return;

   SQLSMALLINT count;

   RETCODE rc = SQLNumResultCols( m_stmt->hstmt, &count );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_stmt->columns.resize( count );

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

statement_base::buffer & statement_base::check_parameter( int index )
{
   static constexpr char operation[] = "MSSQL statement parameter check";

   reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_parameters.size() )
      throw_error( operation, "Too many parameters" );

   buffer & buffer = m_buffers[ index ];

   return buffer;
}

//-----------------------------------------------------------------------------

template<> void statement_base::bind_parameter< std::string >( int index, int c_type, const std::string & t )
{
   static constexpr char operation[] = "MSSQL statement parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   int size = std::min( t.length(), desc.size ) + 1;

   buffer.resize( desc.size + 1 );

   RETCODE rc = SQLBindParameter( m_stmt->hstmt,
                                  index + 1,
                                  SQL_PARAM_INPUT,
                                  c_type,
                                  desc.type,
                                  desc.size,
                                  desc.digits,
                                  buffer.data< void >(),
                                  size,
                                  nullptr );

   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   strncpy_s( buffer.data< char >(), size, t.c_str(), size );
}

//-----------------------------------------------------------------------------

void statement_base::bind_time( int index, time_t t )
{
   std::string time;

   struct tm tm;

   util::time::gmtime( &t, &tm );

   if ( t < 60 * 60 * 24 )
   {
      time.resize( 9 );
      util::time::format_iso_8601_time( &tm, const_cast< char * >( time.c_str() ) );
   }
   else
   {
      if ( t % ( 60 * 60 * 24 ) == 0 )
      {
         time.resize( 11 );
         util::time::format_iso_8601_date( &tm, const_cast< char * >( time.c_str() ) );
      }
      else
      {
         time.resize( 20 );
         util::time::format_iso_8601( &tm, const_cast< char * >( time.c_str() ) );
      }
   }

   set_parameter( index, time );
}

//-----------------------------------------------------------------------------

template< typename T > void statement_base::bind_parameter( int index, int c_type, const T & t )
{
   static constexpr char operation[] = "MSSQL statement parameter bind";

   buffer & buffer = check_parameter( index );

   stmt_t::desc_t & desc( m_parameters[ index ] );

   if ( desc.type == sql_time_type )
      return bind_time( index, static_cast< time_t >( t ) );

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
   static constexpr char operation[] = "MSSQL statement reset";

   if ( m_state == Preparing )
      return;

   RETCODE rc = SQLCancel( m_stmt->hstmt );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   rc = SQLFreeStmt( m_stmt->hstmt, SQL_UNBIND );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

uint64_t statement_base::execute( void )
{
   static constexpr char operation[] = "MSSQL statement execute";

   reset();

   RETCODE rc = SQLExecute( m_stmt->hstmt );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_state = Executed;

   prepare_result_desc();

   uint64_t res = 0;

   if ( m_stmt->columns.size() )
   {
      if ( m_stmt->columns.size() != 1 )
         throw_error( operation, "Too many result columns" );

      mssql::result result( m_stmt );

      result.get_column( 0, res );
   }
   
   reset();
 
   return res;
}

//-----------------------------------------------------------------------------

db::result statement_base::result( void )
{
   static constexpr char operation[] = "MSSQL statement result";

   reset();

   RETCODE rc = SQLExecute( m_stmt->hstmt );
   check_status( operation, m_stmt->hstmt, SQL_HANDLE_STMT, rc );

   m_state = Executed;

   prepare_result_desc();

   return std::make_shared< mssql::result >( m_stmt );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
