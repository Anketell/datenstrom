//-----------------------------------------------------------------------------

#include <sqlite/row.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

row::row( sqlite3_stmt * stmt )
{
   m_stmt  = stmt;
   m_count = sqlite3_column_count( m_stmt );
}

//-----------------------------------------------------------------------------

void row::check_column( int index, int type )
{
   if ( !m_stmt )
      throw std::runtime_error( "Bad row" );

   if ( index >= m_count )
      throw std::runtime_error( "No column available" );

   int column_type = sqlite3_column_type( m_stmt, index );
   if ( column_type != type )
      throw std::runtime_error( "Incorrect column type" );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int8_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int16_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int32_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, int64_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int64( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint8_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint16_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint32_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, uint64_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int64( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, double & d )
{
   check_column( index, SQLITE_FLOAT );
   d = sqlite3_column_double( m_stmt, index );
}

//-----------------------------------------------------------------------------

void row::get_column( int index, std::string & s )
{
   check_column( index, SQLITE_TEXT );
   s = ( const char * )sqlite3_column_text( m_stmt, index );
}

//-----------------------------------------------------------------------------

bool row::step( void )
{
   int rc = sqlite3_step( m_stmt );
   if ( rc != SQLITE_ROW )
   {
      m_stmt = nullptr;
      return false;
   }

   return true;
}

//-----------------------------------------------------------------------------

row::operator bool ( void ) const
{
   return m_stmt;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
