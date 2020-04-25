//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <firebird/positional_statement.h>
#include <firebird/result.h>
#include <firebird/error.h>
#include <firebird/transactional.h>
#include <assert.h>
#include <memory.h>
#include <functional>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

typedef std::function< void ( transactional & ) > guard_fn;

//-----------------------------------------------------------------------------

static void guard( transactional & trans, guard_fn fn )
{
   if ( !trans.tr_handle )
   {
      db::transaction transaction( trans );
      fn( trans );
   }
   else
      fn( trans );
}

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

   guard( m_transactional, [ & ]( transactional & trans ) -> void
   {
      isc_dsql_prepare( status,
                        &trans.tr_handle,
                        &m_stmt->stmt,
                        sql.length(),
                        sql.c_str(), 3, nullptr );
   } );

   check_status( operation, status );

   XSQLDA xsqlda;

   xsqlda.version = SQLDA_VERSION1;
   xsqlda.sqln    = 1;

   isc_dsql_describe_bind( status, &m_stmt->stmt, 1, &xsqlda );

   check_status( operation, status );

   m_xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( xsqlda.sqld ) ) );

   if ( xsqlda.sqld > xsqlda.sqln )
   {
      m_xsqlda->version = SQLDA_VERSION1;
      m_xsqlda->sqln    = xsqlda.sqld;
      isc_dsql_describe_bind( status, &m_stmt->stmt, 3, m_xsqlda );

      check_status( operation, status );
   }
   else
   {
      memcpy( m_xsqlda, &xsqlda, XSQLDA_LENGTH( xsqlda.sqld ) );
      m_xsqlda->sqln = m_xsqlda->sqld;
   }

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

positional_statement::~positional_statement( void )
{
   if ( m_xsqlda )
   {
      for ( int i = 0; i < m_xsqlda->sqld; i++ )
         free( m_xsqlda->sqlvar[ i ].sqldata );

      free( m_xsqlda );
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

void positional_statement::set_parameter( int index, int8_t i )
{
   set_parameter( index, static_cast< int16_t >( i ) );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int16_t i )
{
   check_parameter( index );

   int16_t * data = new int16_t( i );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_SHORT;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int32_t i )
{
   check_parameter( index );

   int32_t * data = new int32_t( i );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_LONG;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, int64_t i )
{
   check_parameter( index );

   int64_t * data = new int64_t( i );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_INT64;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint8_t u )
{
   set_parameter( index, static_cast< uint16_t >( u ) );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint16_t u )
{
   check_parameter( index );

   uint16_t * data = new uint16_t( u );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_SHORT;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint32_t u )
{
   check_parameter( index );

   uint32_t * data = new uint32_t( u );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_LONG;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, uint64_t u )
{
   check_parameter( index );

   uint64_t * data = new uint64_t( u );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_INT64;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, double d )
{
   check_parameter( index );

   double * data = new double( d );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   param.sqltype = SQL_DOUBLE;
   param.sqldata = reinterpret_cast< char * >( data );
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const std::string & s )
{
   check_parameter( index );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   char * data = new char[ param.sqllen ];

   strncpy( data, s.c_str(), param.sqllen );

   param.sqltype = SQL_TEXT;
   param.sqldata = data;
}

//-----------------------------------------------------------------------------

void positional_statement::set_parameter( int index, const char * s )
{
   check_parameter( index );

   XSQLVAR & param( m_xsqlda->sqlvar[ index ] );

   char * data = new char[ param.sqllen ];

   strncpy( data, s, param.sqllen );

   param.sqltype = SQL_TEXT;
   param.sqldata = data;
}

//-----------------------------------------------------------------------------

int positional_statement::parameter_count( void )
{
   return m_xsqlda->sqld;
}

//-----------------------------------------------------------------------------

void positional_statement::reset( void )
{
   m_state = Preparing;
}

//-----------------------------------------------------------------------------

void positional_statement::internal_execute( void )
{
   static constexpr char operation[] = "Firebird positional statement execute";

   ISC_STATUS status[ status_vector_length ];

   guard( m_transactional, [ & ]( transactional & trans ) -> void
   {
      isc_dsql_execute( status,
                        &trans.tr_handle,
                        &m_stmt->stmt,
                        m_xsqlda->sqld,
                        m_xsqlda );
   } );

   check_status( operation, status );
}

//-----------------------------------------------------------------------------

uint64_t positional_statement::execute( void )
{
   static constexpr char operation[] = "Firebird positional statement execute";

   internal_execute();

   uint64_t res = 0;

   firebird::result result( m_stmt );

   if ( result )
   {
      if ( result.column_count() != 1 )
         throw_error( operation, "Too many result columns" );

      result.get_column( 0, res );
   }

   reset();

   return res;
}

//-----------------------------------------------------------------------------

db::result positional_statement::result( void )
{
   db::result result = db::result( nullptr );

   m_state = Executed;

   return result;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
