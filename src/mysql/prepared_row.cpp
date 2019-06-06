//-----------------------------------------------------------------------------

#include <mysql/prepared_row.h>
#include <mysql/error.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

prepared_row::prepared_row( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   m_good = mysql_stmt_fetch( m_stmt->stmt ) == 0;
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int              index,
                               enum_field_types type,
                               void           * p,
                               size_t           length,
                               int              is_unsigned )
{
   MYSQL_BIND column = { 0 };

   column.buffer_type   = type;
   column.buffer_length = length;
   column.buffer        = p;
   column.is_unsigned   = is_unsigned;
   column.is_null       = 0;

   int rc = mysql_stmt_fetch_column( m_stmt->stmt, &column, index, 0 );
   if ( rc )
      throw_error( "MySQL get result column", mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, int8_t & i )
{
   get_column( index, MYSQL_TYPE_TINY, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, int16_t & i )
{
   get_column( index, MYSQL_TYPE_SHORT, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, int32_t & i )
{
   get_column( index, MYSQL_TYPE_LONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, int64_t & i )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &i, sizeof( i ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, uint8_t & u )
{
   get_column( index, MYSQL_TYPE_TINY, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, uint16_t & u )
{
   get_column( index, MYSQL_TYPE_SHORT, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, uint32_t & u )
{
   get_column( index, MYSQL_TYPE_LONG , &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, uint64_t & u )
{
   get_column( index, MYSQL_TYPE_LONGLONG, &u, sizeof( u ), 1 );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, double & d )
{
   get_column( index, MYSQL_TYPE_DOUBLE, &d, sizeof( d ) );
}

//-----------------------------------------------------------------------------

void prepared_row::get_column( int index, std::string & s )
{
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
      throw_error( "MySQL get result column", mysql_stmt_error( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

bool prepared_row::step( void )
{
   m_good = mysql_stmt_fetch( m_stmt->stmt ) == 0;
   return m_good;
}

//-----------------------------------------------------------------------------

prepared_row::operator bool ( void ) const
{
   return m_good;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
