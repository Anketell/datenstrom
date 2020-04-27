//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <firebird/positional_statement.h>
#include <firebird/result.h>
#include <firebird/error.h>
#include <firebird/guard.h>

#include <assert.h>
#include <memory.h>
#include <functional>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

positional_statement::positional_statement( transactional     & trans,
                                            const std::string & sql   ) :
m_transactional( trans )
{
   static constexpr char operation[] = "Firebird positional statement prepare";

   ISC_STATUS status[ status_vector_length ];

   m_stmt = std::make_shared< stmt_t >();

   isc_dsql_allocate_statement( status, &m_transactional.db_handle, &m_stmt->stmt );

   check_status( operation, status );

   guard( m_transactional, [ & ]( void ) -> void
   {
      isc_dsql_prepare( status,
                        &m_transactional.tr_handle,
                        &m_stmt->stmt,
                        sql.length(),
                        sql.c_str(), 3, nullptr );
   } );

   check_status( operation, status );

   prepare_statement_type();
   prepare_parameter_buffer();
   prepare_result_buffer();

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

positional_statement::~positional_statement( void )
{
   if ( m_xsqlda )
   {
      for ( int i = 0; i < m_xsqlda->sqld; i++ )
      {
         free( m_xsqlda->sqlvar[ i ].sqldata );
         free( m_xsqlda->sqlvar[ i ].sqlind );
      }

      free( m_xsqlda );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::prepare_statement_type( void )
{
   static constexpr char operation[] = "Firebird prepare statement type";

   ISC_STATUS status[ status_vector_length ];

   char info[] = { isc_info_sql_stmt_type };
   char res[ 10 ];

   isc_dsql_sql_info( status,
                      &m_stmt->stmt,
                      sizeof( info ), info,
                      sizeof( res ), res );

   check_status( operation, status );

   m_stmt->type = res[ 3 ];
}

//-----------------------------------------------------------------------------

void positional_statement::prepare_parameter_buffer( void )
{
   static constexpr char operation[] = "Firebird positional statement prepare";

   ISC_STATUS status[ status_vector_length ];

   XSQLDA * xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( 1 ) ) );

   xsqlda->version = SQLDA_VERSION1;
   xsqlda->sqln    = 1;

   isc_dsql_describe_bind( status, &m_stmt->stmt, 3, xsqlda );

   check_status( operation, status );

   m_xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( xsqlda->sqld ) ) );

   if ( xsqlda->sqld > xsqlda->sqln )
   {
      m_xsqlda->version = SQLDA_VERSION1;
      m_xsqlda->sqln    = xsqlda->sqld;

      free( xsqlda );

      isc_dsql_describe_bind( status, &m_stmt->stmt, 3, m_xsqlda );

      check_status( operation, status );
   }
   else
      m_xsqlda = xsqlda;

   for ( int i = 0; i < m_xsqlda->sqld; i++ )
   {
      XSQLVAR & param( m_xsqlda->sqlvar[ i ] );
      param.sqldata = reinterpret_cast< char * >( malloc( param.sqllen ) );
      param.sqlind  = new int16_t( 0 );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::prepare_result_buffer( void )
{
   static constexpr char operation[] = "Firebird positional statement prepare";

   ISC_STATUS status[ status_vector_length ];

   XSQLDA * xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( 1 ) ) );

   xsqlda->version = SQLDA_VERSION1;
   xsqlda->sqln    = 1;

   isc_dsql_describe( status, &m_stmt->stmt, 3, xsqlda );

   check_status( operation, status );

   if ( xsqlda->sqld > xsqlda->sqln )
   {
      m_stmt->xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( xsqlda->sqld ) ) );

      m_stmt->xsqlda->version = SQLDA_VERSION1;
      m_stmt->xsqlda->sqln    = xsqlda->sqld;

      free( xsqlda );

      isc_dsql_describe( status, &m_stmt->stmt, 3, m_stmt->xsqlda );

      check_status( operation, status );
   }
   else
      m_stmt->xsqlda = xsqlda;

   for ( int i = 0; i < m_stmt->xsqlda->sqld; i++ )
   {
      XSQLVAR & column( m_stmt->xsqlda->sqlvar[ i ] );
      column.sqldata = reinterpret_cast< char * >( malloc( column.sqllen ) );
      column.sqlind  = new int16_t( 0 );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::check_parameter( int index )
{
   static constexpr char operation[] = "Firebird positional statement parameter check";

   if ( m_state == Executed )
      reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_xsqlda->sqld )
      throw_error( operation, "Too many parameters" );
}

//-----------------------------------------------------------------------------

static constexpr char bind_parameter[] = "Firebird positonal statement parameter bind";

//-----------------------------------------------------------------------------

template< typename BI > void positional_statement::set_big_int( int index, BI bi )
{
   check_parameter( index );

   typedef std::numeric_limits< BI > limits;

   typedef typename std::conditional< limits::is_signed, int16_t, uint16_t >::type SI;
   typedef typename std::conditional< limits::is_signed, int32_t, uint32_t >::type LI;

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   switch ( param.sqltype & ~1 )
   {
      case SQL_SHORT:
      {
         typedef std::numeric_limits< SI > limits;

         if ( bi < limits::min() || bi > limits::max() )
            throw_error( bind_parameter, "Out of numeric range of SQL_SHORT" );

         *reinterpret_cast< SI * >( param.sqldata ) = static_cast< SI >( bi );
         break;
      }

      case SQL_LONG:
      {
         typedef std::numeric_limits< LI > limits;

         if ( bi < limits::min() || bi > limits::max() )
            throw_error( bind_parameter, "Out of numeric range of SQL_LONG" );

         *reinterpret_cast< LI * >( param.sqldata ) = static_cast< LI >( bi );
         break;
      }

      case SQL_INT64:
         *reinterpret_cast< BI * >( param.sqldata ) = bi;
         break;

      case SQL_FLOAT:
      case SQL_DOUBLE:
         set_parameter( index, static_cast< double >( bi ) );
         break;

      default:
         throw_error( bind_parameter, "Not numeric type" );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int8_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int16_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int32_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int64_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint8_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint16_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint32_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint64_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, double d )
{
   check_parameter( index );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   switch ( param.sqltype & ~1 )
   {
      case SQL_FLOAT:
         *reinterpret_cast< float * >( param.sqldata ) = static_cast< float >( d );
         break;

      case SQL_DOUBLE:
         *reinterpret_cast< double * >( param.sqldata ) = d;
         break;

      default:
         throw_error( bind_parameter, "Not floating point type" );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::set_string( int index, uint32_t len, const char * data )
{
   check_parameter( index );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   len = std::min( len, static_cast< uint32_t >( param.sqllen ) );

   switch ( param.sqltype & ~1 )
   {
      case SQL_TEXT:
         memcpy( param.sqldata, data, len );
         break;

      case SQL_VARYING:
         *reinterpret_cast< int16_t * >( param.sqldata ) = static_cast< int16_t >( len );
         memcpy( param.sqldata + sizeof( int16_t ), data, len );
         break;

      default:
         throw_error( bind_parameter, "Not character type" );
   }
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const std::string & s )
{
   set_string( index, s.length(), s.data() );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const char * s )
{
   set_string( index, strlen( s ), s );
}

//-----------------------------------------------------------------------------

int positional_statement::parameter_count( void )
{
   return m_xsqlda->sqld;
}

//-----------------------------------------------------------------------------

void positional_statement::reset( void )
{
//   ISC_STATUS status[ status_vector_length ];

//   isc_dsql_free_statement( status, &m_stmt->stmt, DSQL_close );

//   check_status( "Fierbird drop statement", status );

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

void positional_statement::execute_statement( void )
{
   static constexpr char operation[] = "Firebird positional statement execute";

   ISC_STATUS status[ status_vector_length ];

   isc_dsql_execute( status,
                     &m_transactional.tr_handle,
                     &m_stmt->stmt,
                     1,
                     m_xsqlda );

   check_status( operation, status );
}

//-----------------------------------------------------------------------------

void positional_statement::execute_procedure( void )
{
   static constexpr char operation[] = "Firebird positional statement execute";

   ISC_STATUS status[ status_vector_length ];

   isc_dsql_execute2( status,
                      &m_transactional.tr_handle,
                      &m_stmt->stmt,
                      1,
                      m_xsqlda,
                      m_stmt->xsqlda );

   check_status( operation, status );
}

//-----------------------------------------------------------------------------

void positional_statement::internal_execute( void )
{
   if  ( m_stmt->type == isc_info_sql_stmt_exec_procedure )
      execute_procedure();
   else
      execute_statement();

   m_state = Executed;
}

//-----------------------------------------------------------------------------

uint64_t positional_statement::execute( void )
{
   static constexpr char operation[] = "Firebird positional statement execute";

   uint64_t res = 0;

   guard( m_transactional, [ & ]( void ) -> void
   {
      internal_execute();

      if ( m_stmt->type != isc_info_sql_stmt_ddl )
      {
         firebird::result result( m_stmt );

         if ( result )
         {
            if ( result.column_count() != 1 )
               throw_error( operation, "Too many result columns" );

            result.get_column( 0, res );
         }
      }
   } );

   reset();

   return res;
}

//-----------------------------------------------------------------------------

db::result positional_statement::result( void )
{
   std::unique_ptr< ds::db::transaction > transaction;

   if ( !m_transactional.tr_handle )
      transaction = std::make_unique< ds::db::transaction >( m_transactional );

   internal_execute();

   return db::result( std::make_shared< firebird::result >( m_stmt, std::move( transaction ) ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
