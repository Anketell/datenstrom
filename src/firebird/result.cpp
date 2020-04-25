//-----------------------------------------------------------------------------

#include <firebird/result.h>
#include <firebird/error.h>
#include <memory.h>
#include <limits>
#include <type_traits>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   static constexpr char operation[] = "Firebird result prepare";

   ISC_STATUS status[ status_vector_length ];

   XSQLDA xsqlda;

   xsqlda.version = SQLDA_VERSION1;
   xsqlda.sqln    = 1;

   isc_dsql_describe( status, &m_stmt->stmt, 1, &xsqlda );

   check_status( operation, status );

   m_xsqlda = reinterpret_cast< XSQLDA * >( malloc( XSQLDA_LENGTH( xsqlda.sqld ) ) );

   if ( xsqlda.sqld > xsqlda.sqln )
   {
      m_xsqlda->version = SQLDA_VERSION1;
      m_xsqlda->sqln    = xsqlda.sqld;
      isc_dsql_describe( status, &m_stmt->stmt, 3, m_xsqlda );

      check_status( operation, status );
   }
   else
   {
      memcpy( m_xsqlda, &xsqlda, XSQLDA_LENGTH( xsqlda.sqld ) );
      m_xsqlda->sqln = m_xsqlda->sqld;
   }

   for ( int i = 0; i < m_xsqlda->sqld; i++ )
   {
      XSQLVAR & column( m_xsqlda->sqlvar[ i ] );
      column.sqldata = reinterpret_cast< char * >( malloc( column.sqllen ) );
   }

   step();
}

//-----------------------------------------------------------------------------

result::~result( void )
{
   if ( m_xsqlda )
   {
      for ( int i = 0; i < m_xsqlda->sqld; i++ )
         free( m_xsqlda->sqlvar[ i ].sqldata );

      free( m_xsqlda );
   }
}

//-----------------------------------------------------------------------------

int result::column_count( void ) const
{
   return m_xsqlda->sqln;
}

//-----------------------------------------------------------------------------

int result::rows_affected( void ) const
{
   return 0;
}

//-----------------------------------------------------------------------------

static constexpr char operation[] = "Firebird get result column";

//-----------------------------------------------------------------------------

template< typename BI > BI result::get_big_int( int index )
{
   typedef std::numeric_limits< BI > limits;

   typedef typename std::conditional< limits::is_signed, int16_t, uint16_t >::type SI;
   typedef typename std::conditional< limits::is_signed, int32_t, uint32_t >::type LI;

   BI bi;

   XSQLVAR & column( m_xsqlda->sqlvar[ index ] );

   switch ( column.sqltype )
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

      default:
         throw_error( operation, "Not integer type" );
   }

   return bi;
}

//-----------------------------------------------------------------------------

template< typename I > I result::get_integer( int index )
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

void result::get_column( int index, int8_t & i )
{
   i = get_integer< int8_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int16_t & i )
{
   i = get_integer< int16_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int32_t & i )
{
   i = get_integer< int32_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int64_t & i )
{
   i = get_big_int< int64_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint8_t & u )
{
   u = get_integer< uint8_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint16_t & u )
{
   u = get_integer< uint16_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint32_t & u )
{
   u = get_integer< uint32_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint64_t & u )
{
   u = get_big_int< uint64_t >( index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, double & d )
{
   throw_error( operation, "Not implemeneted" );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, std::string & s )
{
   throw_error( operation, "Not implemeneted" );
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   ISC_STATUS status[ status_vector_length ];

   m_valid = !isc_dsql_fetch( status, &m_stmt->stmt, 1, m_xsqlda );

   return *this;
}

//-----------------------------------------------------------------------------

result::operator bool ( void ) const
{
   return m_valid;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
