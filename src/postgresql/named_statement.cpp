//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <postgresql/named_statement.h>
#include <postgresql/rowset.h>
#include <postgresql/error.h>
#include <db/parameter.h>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <array>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

named_statement::named_statement( PGconn                * conn,
                                  const std::string     & sql,
                                  const db::name_list_t & parameters ) :
statement_base( conn ),
m_names( parameters )
{
   std::string pos_sql = get_pos_sql( sql, m_names );
   prepare( pos_sql );
}

//-----------------------------------------------------------------------------

std::string named_statement::get_pos_sql( const std::string     & sql,
                                          const db::name_list_t & parameters )
{
   static constexpr char operation[] = "PostgreSQL prepare named statement";

   std::vector< bool > param_seen( parameters.size() );

   std::string pos_sql;

   int unique = 0;

   uint32_t from = 0;
   for ( auto parameter : db::parameter::enumerator( sql, ":@" ) )
   {
      std::string name = sql.substr( parameter.from + 1, parameter.len - 1 );

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

      pos_sql += sql.substr( from, parameter.from - from ) + "$" + std::to_string( j + 1 );

      from = parameter.from + parameter.len;

      if ( !param_seen[ j ] )
      {
         param_seen[ j ] = true;
         unique++;
      }
   }

   pos_sql += sql.substr( from );

   if ( unique != parameters.size() )
      throw_error( operation, "Wrong number of parameters" );

   return pos_sql;
}

//-----------------------------------------------------------------------------

int named_statement::parameter_count( void )
{
   return static_cast< int >( m_names.size() );
}

//-----------------------------------------------------------------------------

}