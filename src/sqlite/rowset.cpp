//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/rowset.h>
#include <sqlite/error.h>
#include <dsutil/timestamp.h>
#include <unordered_map>
#include <cstring>

//-----------------------------------------------------------------------------

#define SQLITE_TIME     ( SQLITE_NULL + 1 )
#define SQLITE_DATETIME ( SQLITE_NULL + 2 )

//-----------------------------------------------------------------------------

namespace ds::sqlite
{

//-----------------------------------------------------------------------------

rowset::rowset( std::shared_ptr< stmt_t > stmt ) :
m_stmt( stmt )
{
   if ( step() )
      m_count = sqlite3_column_count( m_stmt->stmt );
   else
      m_count = 0;
}

//-----------------------------------------------------------------------------

rowset::~rowset( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

int rowset::column_count( void ) const
{
   return m_count;
}

//-----------------------------------------------------------------------------

int rowset::rows_affected( void ) const
{
   return sqlite3_changes( sqlite3_db_handle( m_stmt->stmt ) );
}

//-----------------------------------------------------------------------------

int rowset::check_column( int index, int type_mask )
{
   static constexpr char operation[] = "SQLite rowset get column";

   static std::unordered_map< std::string, int > type_map =
   {
      { "BLOB",     SQLITE_BLOB     },
      { "TIME",     SQLITE_TIME     },
      { "DATETIME", SQLITE_DATETIME }
   };

   if ( !m_valid )
      throw_error( operation, "No row available" );

   if ( !m_stmt )
      throw_error( operation, "Bad rowset" );

   if ( index >= m_count )
      throw_error( operation, "No column available" );

   int column_type = sqlite3_column_type( m_stmt->stmt, index );
   if ( column_type == SQLITE_TEXT )
   {
      const char * decl_type = sqlite3_column_decltype( m_stmt->stmt, index );

      auto it = type_map.find( decl_type );
      if ( it != type_map.end() )
         column_type = it->second;
   }

   if ( ( ( 1 << column_type ) & type_mask ) == 0 )
      throw_error( operation, "Incorrect column type" );

   return column_type;
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int8_t & i )
{
   check_column( index, 1 << SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int16_t & i )
{
   check_column( index, 1 << SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int32_t & i )
{
   check_column( index, 1 << SQLITE_INTEGER );
   i = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, int64_t & i )
{
   check_column( index, 1 << SQLITE_INTEGER );
   i = sqlite3_column_int64( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint8_t & u )
{
   check_column( index, 1 << SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint16_t & u )
{
   check_column( index, 1 << SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint32_t & u )
{
   check_column( index, 1 << SQLITE_INTEGER );
   u = sqlite3_column_int( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, uint64_t & u )
{
   check_column( index, 1 << SQLITE_INTEGER );
   u = sqlite3_column_int64( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, double & d )
{
   check_column( index, 1 << SQLITE_FLOAT );
   d = sqlite3_column_double( m_stmt->stmt, index );
}

//-----------------------------------------------------------------------------

void rowset::get_column( int index, std::string & s )
{
   static constexpr int column_mask = ( 1 << SQLITE_TEXT ) |
                                      ( 1 << SQLITE_BLOB ) |
                                      ( 1 << SQLITE_TIME ) |
                                      ( 1 << SQLITE_DATETIME );

   int column_type = check_column( index, column_mask );

   int length = sqlite3_column_bytes( m_stmt->stmt, index );

   const void * p;
   switch ( column_type )
   {
      case SQLITE_TEXT:
         p = sqlite3_column_text( m_stmt->stmt, index );
         s.assign( reinterpret_cast< const char * >( p ), length );
         break;

      case SQLITE_BLOB:
         p = sqlite3_column_blob( m_stmt->stmt, index );
         s.assign( reinterpret_cast< const char * >( p ), length );
         break;

      case SQLITE_TIME:
         p = sqlite3_column_text( m_stmt->stmt, index );
         s.assign( reinterpret_cast< const char * >( p ), length );
         ds::time::reformat_iso_8601_time( s );
         break;

      case SQLITE_DATETIME:
         p = sqlite3_column_text( m_stmt->stmt, index );
         s.assign( reinterpret_cast< const char * >( p ), length );
         ds::time::reformat_iso_8601( s );
         break;
   }
}

//-----------------------------------------------------------------------------

bool rowset::step( void )
{
   m_valid = sqlite3_step( m_stmt->stmt ) == SQLITE_ROW;

   return !this->eof();
}

//-----------------------------------------------------------------------------

bool rowset::eof( void ) const
{
   return !m_valid;
}

//-----------------------------------------------------------------------------

}
