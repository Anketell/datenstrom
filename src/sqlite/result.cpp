//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/result.h>
#include <sqlite/error.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

result::result( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   if ( step() )
      m_count = sqlite3_column_count( m_stmt->stmt );
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
   return sqlite3_changes( sqlite3_db_handle( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

void result::check_column( int index, int type )
{
   static constexpr char operation[] = "SQLite result column check";

   if ( !m_stmt )
      throw_error( operation, "Bad result" );

   if ( index >= m_count )
      throw_error( operation, "No column available" );

   int column_type = sqlite3_column_type( m_stmt->stmt, index );
   if ( column_type != type )
      throw_error( operation, "Incorrect column type" );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int8_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int16_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int32_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, int64_t & i )
{
   check_column( index, SQLITE_INTEGER );
   i = sqlite3_column_int64( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint8_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint16_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint32_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, uint64_t & u )
{
   check_column( index, SQLITE_INTEGER );
   u = sqlite3_column_int64( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, double & d )
{
   check_column( index, SQLITE_FLOAT );
   d = sqlite3_column_double( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void result::get_column( int index, std::string & s )
{
   check_column( index, SQLITE_TEXT );
   s = ( const char * )sqlite3_column_text( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

bool result::step( void )
{
   m_valid = sqlite3_step( m_stmt->stmt ) == SQLITE_ROW;

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool result::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
