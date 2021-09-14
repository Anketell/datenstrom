//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/statement.h>
#include <sqlite/result.h>
#include <sqlite/error.h>
#include <cassert>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

statement::statement( sqlite3 * db, const std::string     & sql,
                                    const db::name_list_t & parameters )
{
   static constexpr char operation[] = "SQLite statement prepare";

   m_db = db;

   m_stmt = std::make_shared< stmt_t >();

   int rc = sqlite3_prepare_v2( m_db, sql.c_str(), sql.length(), &m_stmt->stmt, nullptr );
   if ( rc )
      throw_error( operation, rc );

   m_state = Preparing;

   m_count = sqlite3_bind_parameter_count( m_stmt->stmt );

   if ( parameters.size() )
   {
      if ( parameters.size() != m_count )
         throw_error( operation, "Named parameters don't match statement" );

      m_parameters.reserve( m_count );
      for ( auto & name : parameters )
      {
         int index = 0;

         constexpr char prefix[] = ":@$";
         for ( auto c : prefix )
         {
            index = sqlite3_bind_parameter_index( m_stmt->stmt, ( c + name ).c_str() );
            if ( index )
               break;
         }

         if ( index == 0 )
            throw_error( operation, "Unmatched named parameter" );

         m_parameters.push_back( index );
      }
   }
}

//-----------------------------------------------------------------------------

int statement::check_parameter( int index )
{
   static constexpr char operation[] = "SQLite statement parameter check";

   reset();

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_count )
      throw_error( operation, "Too many parameters" );

   if ( m_parameters.empty() )
      return index + 1;

   return m_parameters[ index ];
}

//-----------------------------------------------------------------------------

static constexpr char bind_parameter[] = "SQLite statement parameter bind";

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int8_t i )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), i );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int16_t i )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), i );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int32_t i )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), i );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int64_t i )
{
   int rc = sqlite3_bind_int64( m_stmt->stmt, check_parameter( index ), i );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint8_t u )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), u );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint16_t u )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), u );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint32_t u )
{
   int rc = sqlite3_bind_int( m_stmt->stmt, check_parameter( index ), u );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint64_t u )
{
   int rc = sqlite3_bind_int64( m_stmt->stmt, check_parameter( index ), u );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, double d )
{
   int rc = sqlite3_bind_double( m_stmt->stmt, check_parameter( index ), d );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, const std::string & s )
{
   int rc = sqlite3_bind_text( m_stmt->stmt,
                               check_parameter( index ),
                               s.c_str(), -1,
                               SQLITE_STATIC );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, const char * s )
{
   int rc = sqlite3_bind_text( m_stmt->stmt,
                               check_parameter( index ),
                               s, -1,
                               SQLITE_STATIC );
   if ( rc )
      throw_error( bind_parameter, rc );
}

//-----------------------------------------------------------------------------

int statement::parameter_count( void )
{
   return m_count;
}

//-----------------------------------------------------------------------------

void statement::reset( void )
{
   if ( m_state == Preparing )
      return;

   int rc = sqlite3_reset( m_stmt->stmt );
   if ( rc != SQLITE_OK )
      throw_error( "SQLite statement reset", rc );

   rc = sqlite3_clear_bindings( m_stmt->stmt );
   assert( rc == SQLITE_OK );

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

uint64_t statement::execute( void )
{
   static constexpr char operation[] = "SQLite statement execute";

   uint64_t res = 0;

   reset();

   sqlite::result result( m_stmt );

   m_state = Executed;

   if ( result )
   {
      if ( result.column_count() != 1 )
         throw_error( operation, "Too many result columns" );

      result.get_column( 0, res );
   }
   else
      res = sqlite3_last_insert_rowid( m_db );

   reset();

   return res;
}

//-----------------------------------------------------------------------------

db::result statement::result( void )
{
   reset();

   db::result result = db::result( std::make_shared< sqlite::result >( m_stmt ) );

   m_state = Executed;

   return result;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
