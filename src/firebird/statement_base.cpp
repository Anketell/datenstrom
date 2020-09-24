//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <firebird/statement_base.h>
#include <firebird/result.h>
#include <firebird/error.h>
#include <firebird/guard.h>

#include <memory.h>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

statement_base::statement_base( transactional & trans ) :
m_transactional( trans )
{
}

//-----------------------------------------------------------------------------

statement_base::~statement_base( void )
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

void statement_base::prepare( const std::string & sql )
{
   static constexpr char operation[] = "Firebird statement prepare";

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

   m_stmt->state = stmt_t::Preparing;
}

//-----------------------------------------------------------------------------

int statement_base::get_statement_type( isc_stmt_handle stmt )
{
   static constexpr char operation[] = "Firebird get statement type";

   ISC_STATUS status[ status_vector_length ];

   char info[] = { isc_info_sql_stmt_type };
   char res[ 10 ];

   isc_dsql_sql_info( status,
                      &stmt,
                      sizeof( info ), info,
                      sizeof( res ), res );

   check_status( operation, status );

   int32_t len  = isc_vax_integer( res + 1, 2 );
   return isc_vax_integer( res + 3, len );
}

//-----------------------------------------------------------------------------

void statement_base::prepare_statement_type( void )
{
   m_stmt->type = get_statement_type( m_stmt->stmt );
}

//-----------------------------------------------------------------------------

XSQLDA * statement_base::prepare_xsqlda( describe_fn_t describe_fn )
{
   XSQLDA * xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( 1 ) ) );

   try
   {
      xsqlda->version = SQLDA_VERSION1;
      xsqlda->sqln    = 1;

      describe_fn( xsqlda );

      if ( xsqlda->sqld > xsqlda->sqln )
      {
         XSQLDA * x = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( xsqlda->sqld ) ) );

         x->version = SQLDA_VERSION1;
         x->sqln    = xsqlda->sqld;

         free( xsqlda );

         xsqlda = x;

         describe_fn( xsqlda );
      }
   }
   catch ( ... )
   {
      free( xsqlda );
      throw;
   }

   return xsqlda;
}

//-----------------------------------------------------------------------------

void statement_base::allocate_xsqlvars( XSQLDA * xsqlda )
{
   for ( int i = 0; i < xsqlda->sqld; i++ )
   {
      XSQLVAR & var( xsqlda->sqlvar[ i ] );

      uint16_t len = ( var.sqltype & ~1 ) == SQL_VARYING ? var.sqllen + 2 : var.sqllen;

      var.sqldata = reinterpret_cast< char * >( malloc( len ) );
      var.sqlind  = reinterpret_cast< int16_t * >( malloc( sizeof( int16_t ) ) );
      *var.sqlind = 0;
   }
}

//-----------------------------------------------------------------------------

XSQLDA *  statement_base::prepare_parameter_xsqlda( isc_stmt_handle stmt )
{
   static constexpr char operation[] = "Firebird statement prepare";

   ISC_STATUS status[ status_vector_length ];

   return prepare_xsqlda( [ & ]( XSQLDA * xsqlda )
   {
      isc_dsql_describe_bind( status, &stmt, 3, xsqlda );
      check_status( operation, status );
   } );
}

//-----------------------------------------------------------------------------

XSQLDA * statement_base::prepare_result_xqslda( isc_stmt_handle stmt )
{
   static constexpr char operation[] = "Firebird statement prepare";

   ISC_STATUS status[ status_vector_length ];

   return prepare_xsqlda( [ & ]( XSQLDA * xsqlda )
   {
      isc_dsql_describe( status, &stmt, 3, xsqlda );
      check_status( operation, status );
   } );
}

//-----------------------------------------------------------------------------

void statement_base::prepare_parameter_buffer( void )
{
   m_xsqlda = prepare_parameter_xsqlda( m_stmt->stmt );
   allocate_xsqlvars( m_xsqlda );
}

//-----------------------------------------------------------------------------

void statement_base::prepare_result_buffer( void )
{
   m_stmt->xsqlda = prepare_result_xqslda( m_stmt->stmt );
   allocate_xsqlvars( m_stmt->xsqlda );
}

//-----------------------------------------------------------------------------

void statement_base::check_parameter( int index )
{
   static constexpr char operation[] = "Firebird statement parameter check";

   reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_xsqlda->sqld )
      throw_error( operation, "Too many parameters" );
}

//-----------------------------------------------------------------------------

static constexpr char bind_parameter[] = "Firebird statement parameter bind";

//-----------------------------------------------------------------------------

template< typename BI > void statement_base::set_big_int( int index, BI bi )
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

      case SQL_TYPE_DATE:
         *reinterpret_cast< ISC_DATE * >( param.sqldata ) = encode_sql_unixdate( bi );
         break;

      default:
         throw_error( bind_parameter, "Not numeric type" );
   }
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int8_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int16_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int32_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int64_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint8_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint16_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint32_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint64_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, double d )
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

void statement_base::set_string( int index, uint32_t len, const char * data )
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

      case SQL_TYPE_DATE:
         *reinterpret_cast< ISC_DATE * >( param.sqldata ) = encode_sql_date( data );
         break;

      default:
         throw_error( bind_parameter, "Not character type" );
   }
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const std::string & s )
{
   set_string( index, s.length(), s.data() );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s )
{
   set_string( index, strlen( s ), s );
}

//-----------------------------------------------------------------------------

int statement_base::parameter_count( void )
{
   return m_xsqlda->sqld;
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   if ( m_stmt->state == stmt_t::Preparing )
      return;

   ISC_STATUS status[ status_vector_length ];

   isc_dsql_free_statement( status, &m_stmt->stmt, DSQL_close );

   check_status( "Fierbird drop statement", status );

   m_stmt->state = stmt_t::Preparing;
}

//-----------------------------------------------------------------------------

void statement_base::execute_statement( void )
{
   static constexpr char operation[] = "Firebird statement execute";

   ISC_STATUS status[ status_vector_length ];

   isc_dsql_execute( status,
                     &m_transactional.tr_handle,
                     &m_stmt->stmt,
                     1,
                     m_xsqlda );

   check_status( operation, status );

   m_stmt->state = stmt_t::Executed;
}

//-----------------------------------------------------------------------------

void statement_base::execute_procedure( void )
{
   static constexpr char operation[] = "Firebird statement execute";

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

void statement_base::internal_execute( void )
{
   reset();

   if  ( m_stmt->type == isc_info_sql_stmt_exec_procedure )
      execute_procedure();
   else
      execute_statement();
}

//-----------------------------------------------------------------------------

uint64_t statement_base::execute( void )
{
   static constexpr char operation[] = "Firebird statement execute";

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
            if ( result )
               reset();
         }
      }
   } );

   return res;
}

//-----------------------------------------------------------------------------

db::result statement_base::result( void )
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
