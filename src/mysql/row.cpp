//-----------------------------------------------------------------------------

#include <mysql/row.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

row::row( MYSQL & mysql ) :
m_mysql( mysql )
{
   m_count       = mysql_field_count( &m_mysql );
   m_mysql_res   = mysql_use_result( &m_mysql );
   m_mysql_field = mysql_fetch_field( m_mysql_res );
   m_mysql_row   = mysql_fetch_row( m_mysql_res );
}

//-----------------------------------------------------------------------------

row::~row( void )
{
   mysql_free_result( m_mysql_res );
}

//-----------------------------------------------------------------------------

void row::check_column( int index, int type )
{
   if ( !m_mysql_res || !m_mysql_row )
      throw std::runtime_error( "Bad row" );

   if ( index >= m_count )
      throw std::runtime_error( "No column available" );

   MYSQL_FIELD * mysql_field = mysql_fetch_field( m_mysql_res );

   if ( mysql_field[ index - 1 ].type != type )
      throw std::runtime_error( "Incorrect column type" );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int8_t & i )
{
   check_column( index, MYSQL_TYPE_TINY );
   i = *reinterpret_cast< int8_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int16_t & i )
{
   check_column( index, MYSQL_TYPE_SHORT );
   i = *reinterpret_cast< int16_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int32_t & i )
{
   check_column( index, MYSQL_TYPE_LONG );
   i = *reinterpret_cast< int32_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int64_t & i )
{
   check_column( index, MYSQL_TYPE_LONGLONG );
   i = *reinterpret_cast< int64_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint8_t & u )
{
   check_column( index, MYSQL_TYPE_TINY );
   u = *reinterpret_cast< uint8_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint16_t & u )
{
   check_column( index, MYSQL_TYPE_SHORT );
   u = *reinterpret_cast< uint16_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint32_t & u )
{
   check_column( index, MYSQL_TYPE_LONG );
   u = *reinterpret_cast< uint32_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint64_t & u )
{
   check_column( index, MYSQL_TYPE_LONGLONG );
   u = *reinterpret_cast< uint64_t * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, double & d )
{
   check_column( index, MYSQL_TYPE_DOUBLE );
   d = *reinterpret_cast< double * >( m_mysql_row[ index ] );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, std::string & s )
{
   check_column( index, MYSQL_TYPE_VAR_STRING );
   s = m_mysql_row[ index ];
}

//-----------------------------------------------------------------------------

bool row::step( void )
{
   m_mysql_row = mysql_fetch_row( m_mysql_res );
   return m_mysql_row;
}

//-----------------------------------------------------------------------------

row::operator bool ( void ) const
{
   return m_mysql_row;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
