//-----------------------------------------------------------------------------

#include <mysql/result.h>
#include <mysql/error.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   if ( step() )
      m_count = mysql_stmt_field_count( m_stmt->stmt );
   else
      m_count = 0;
}

//-----------------------------------------------------------------------------

int result::column_count( void ) const
{
   return m_count;
}

//-----------------------------------------------------------------------------

int result::rows_affected( void ) const
{
   int rows = mysql_stmt_affected_rows( m_stmt->stmt );
   return rows > 0 ? rows : 0;
}

//-----------------------------------------------------------------------------

 void result::get_column( int              index,
                          enum_field_types type,
                          void           * p,
                          size_t           length,
                          int              is_unsigned )
 {
   static constexpr char operation[] = "MySQL get result column";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   MYSQL_BIND column = {};

   column.buffer_type   = type;
   column.buffer_length = length;
   column.buffer        = p;
   column.is_unsigned   = is_unsigned;
   column.is_null       = 0;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int8_t & i )
{
   get_column( index, MYSQL_TYPE_TINY, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int16_t & i )
{
   get_column( index, MYSQL_TYPE_SHORT, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int32_t & i )
{
   get_column( index, MYSQL_TYPE_LONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int64_t & i )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint8_t & u )
{
   get_column( index, MYSQL_TYPE_TINY, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint16_t & u )
{
   get_column( index, MYSQL_TYPE_SHORT, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint32_t & u )
{
   get_column( index, MYSQL_TYPE_LONG , &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint64_t & u )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, double & d )
{
   get_column( index, MYSQL_TYPE_DOUBLE, &d, sizeof( d ) );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, std::string & s )
{
   static constexpr char operation[] = "MySQL get result column";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   unsigned long length = 0;

   MYSQL_BIND column = { 0 };

   column.buffer_type   = MYSQL_TYPE_VAR_STRING;
   column.buffer_length = 0;
   column.length        = &length;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   if ( rc == 0 )
   {
      s.resize( length );

      column.buffer_length = s.length();
      column.buffer        = const_cast< char * >( s.data() );

      rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   }

   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   m_valid = mysql_stmt_fetch( m_stmt->stmt ) == 0;

   return m_valid;
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
