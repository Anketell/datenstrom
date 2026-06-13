//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mysql/named_statement.h>
#include <mysql/rowset.h>
#include <mysql/error.h>
#include <db/parameter.h>
#include <algorithm>
#include <sstream>
#include <cstring>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

named_statement::named_statement( MYSQL                 & mysql,
                                  const std::string     & sql,
                                  const db::name_list_t & parameters ) :
statement_base( mysql ),
m_names( parameters )
{
   std::string pos_sql = get_pos_sql( sql, m_names );
   prepare( pos_sql );
}

//-----------------------------------------------------------------------------

std::string named_statement::get_pos_sql( const std::string     & sql,
                                          const db::name_list_t & parameters )
{
   static constexpr char operation[] = "MySQL prepare named statement";

   std::string pos_sql;

   int unique = 0;
   int param  = 0;

   uint32_t from = 0;
   for ( auto parameter : db::parameter::enumerator( sql, ":$" ) )
   {
      std::string name = sql.substr( parameter.from + 1, parameter.len - 1 );

      pos_sql += sql.substr( from, parameter.from - from ) + '?';

      auto it = std::find( parameters.begin(), parameters.end(), name );
      if ( it == parameters.end() )
      {
         std::stringstream ss;

         ss << std::endl
            << sql << std::endl
            << "Parameter mismatch: " << name;

         throw_error( operation, ss.str().c_str() );
      }

      int j = static_cast< int >( it - parameters.begin() );

      auto it2 = m_param_map.lower_bound( j );
      if ( it2 == m_param_map.end() || it2->first != j )
         unique++;

      m_param_map.insert( { j, param++ } );

      from = parameter.from + parameter.len;
   }

   pos_sql += sql.substr( from );

   if ( unique != parameters.size() )
      throw_error( operation, "Wrong number of parameters" );

   return pos_sql;
}

//-----------------------------------------------------------------------------

void named_statement::check_parameter( int index )
{
   static constexpr char operation[] = "MySQL named statement parameter check";

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_names.size() )
      throw_error( operation, "Too many parameters" );
}

//-----------------------------------------------------------------------------

template< typename T > void named_statement::internal_set_parameter( int index, T t )
{
   check_parameter( index );

   auto begin = m_param_map.lower_bound( index );
   auto end   = m_param_map.upper_bound( index );

   for ( auto it = begin; it != end; it++ )
      statement_base::set_parameter( it->second, t );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int8_t i )
{
   internal_set_parameter< int8_t >( index, i );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int16_t i )
{
   internal_set_parameter< int16_t >( index, i );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int32_t i )
{
   internal_set_parameter< int32_t >( index, i );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int64_t i )
{
   internal_set_parameter< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint8_t u )
{
   internal_set_parameter< uint8_t >( index, u );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint16_t u )
{
   internal_set_parameter< uint16_t >( index, u );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint32_t u )
{
   internal_set_parameter< uint32_t >( index, u );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint64_t u )
{
   internal_set_parameter< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, double d )
{
   internal_set_parameter< double >( index, d );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, const char * s )
{
   internal_set_parameter< const char * >( index, s );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, const std::string & s )
{
   internal_set_parameter< const std::string & >( index, s );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter_null( int index )
{
   check_parameter( index );

   auto begin = m_param_map.lower_bound( index );
   auto end   = m_param_map.upper_bound( index );

   for ( auto it = begin; it != end; it++ )
      statement_base::set_parameter_null( it->second );
}

//-----------------------------------------------------------------------------

int named_statement::parameter_count( void )
{
   return static_cast< int >( m_names.size() );
}

//-----------------------------------------------------------------------------

}