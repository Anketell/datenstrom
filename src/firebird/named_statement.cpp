//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/named_statement.h>
#include <firebird/guard.h>
#include <dsutil/parameter.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <map>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

static constexpr char operation[] = "Firebird prepare named statement";

//-----------------------------------------------------------------------------

named_statement::named_statement( transactional         & transactional,
                                  const std::string     & sql,
                                  const db::name_list_t & parameters ) :
statement_base( transactional )
{
   int32_t type;

   std::string wrapped_sql = wrap_sql( sql, parameters, &type );
   std::cout << wrapped_sql << std::endl;
   prepare( wrapped_sql );

   m_stmt->type = type;
}

//-----------------------------------------------------------------------------

std::string named_statement::get_pos_sql( const std::string     & sql,
                                          const db::name_list_t & parameters,
                                          int                   * index )
{
   for ( int i = 0; i < parameters.size(); i++ )
      index[ i ] = -1;

   std::string pos_sql;

   int unique = 0;
   int param = 0;

   uint32_t from = 0;
   for ( auto parameter : util::parameter::enumerator( sql, ":" ) )
   {
      std::string name = sql.substr( parameter.from + 1, parameter.len - 1 );

      pos_sql += sql.substr( from, parameter.from - from ) + '?';

      auto it = std::find( parameters.begin(), parameters.end(), name );
      if ( it == parameters.end() )
         throw_error( operation, "Parameter mismatch" );

      int j = it - parameters.begin();
      if ( index[ j ] == -1 )
      {
         index[ j ] = param;
         unique++;
      }

      param++;
      from = parameter.from + parameter.len;
   }

   pos_sql += sql.substr( from );

   if ( unique != parameters.size() )
      throw_error( operation, "Wrong number of parameters" );

   return pos_sql;
}

//-----------------------------------------------------------------------------

void named_statement::get_stmt_meta( const std::string     & sql,
                                     const db::name_list_t & parameters,
                                     stmt_meta_t           * meta )
{
   std::string pos_sql = get_pos_sql( sql, parameters, meta->index );

   ISC_STATUS status[ status_vector_length ];

   isc_stmt_handle stmt = 0;

   isc_dsql_allocate_statement( status, &m_transactional.db_handle, &stmt );

   check_status( operation, status );

   guard( m_transactional, [ & ]( void ) -> void
   {
      isc_dsql_prepare( status,
                        &m_transactional.tr_handle,
                        &stmt,
                        pos_sql.length(),
                        pos_sql.data(), 3, nullptr );
   } );

   check_status( operation, status );

   meta->type = get_statement_type( stmt );
   meta->in   = prepare_parameter_xsqlda( stmt );
   meta->out  = prepare_result_xqslda( stmt );

   isc_dsql_free_statement( status, &stmt, DSQL_drop );

   check_status( operation, status );
}

//-----------------------------------------------------------------------------

std::string data_type( const XSQLVAR & xsqlvar )
{
   std::string type;

   switch ( xsqlvar.sqltype & ~1 )
   {
      case SQL_TEXT:
         type = "TEXT( " + std::to_string( xsqlvar.sqllen ) + " )";
         break;

      case SQL_VARYING:
         type = "VARCHAR( " + std::to_string( xsqlvar.sqllen ) + " )";
         break;

      case SQL_SHORT:
         type = "SMALLINT";
         break;

      case SQL_LONG:
         type = "INTEGER";
         break;

      case SQL_FLOAT:
         type = "FLOAT";
         break;

      case SQL_DOUBLE:
         type = "DOUBLE PRECISION";
         break;

      case SQL_INT64:
         type = "BIGINT";
         break;

      case SQL_TYPE_DATE:
         type = "DATE";
         break;

      case SQL_TYPE_TIME:
         type = "TIME";
         break;

      case SQL_TIMESTAMP:
         type = "TIMESTAMP";
         break;

      case SQL_BLOB:
         type = "BLOB";
         break;

      default:
         throw_error( operation, "Unsupported data type" );
   }

   return type;
}

//-----------------------------------------------------------------------------

std::string named_statement::wrap_sql( const std::string     & sql,
                                       const db::name_list_t & parameters,
                                       int32_t               * type )
{
   static constexpr char suffix[] = "_ds_helper";

   int32_t * index = new int32_t[ parameters.size() ];

   stmt_meta_t meta = { index, nullptr, nullptr, 0 };

   get_stmt_meta( sql, parameters, &meta );

   std::stringstream wrapped_sql;

   wrapped_sql << "EXECUTE BLOCK";

   if ( meta.in->sqld )
   {
      wrapped_sql << " ( ";

      int i = 0;
      for ( auto name : parameters )
      {
         if ( i )
            wrapped_sql << ", ";

         wrapped_sql << name << " "
                     << data_type( meta.in->sqlvar[ index[ i++ ] ] ) << " = ?";
      }
      wrapped_sql << " )";
   }

   wrapped_sql << std::endl;

   if ( meta.out->sqld )
   {
      wrapped_sql << "RETURNS ( ";
      for ( int i = 0; i < meta.out->sqld; i++ )
      {
         if ( i )
            wrapped_sql << ", ";

         wrapped_sql << meta.out->sqlvar[ i ].sqlname << suffix << " "
                     << data_type( meta.out->sqlvar[ i ] );
      }
      wrapped_sql << " )" << std::endl;
   }

   wrapped_sql << "AS" << std::endl
               << "BEGIN" << std::endl;

   if ( meta.type == isc_info_sql_stmt_select )
      wrapped_sql << "FOR" << std::endl;

   wrapped_sql << sql;

   if ( meta.out->sqld )
   {
      wrapped_sql << std::endl << "INTO ";
      for ( int i = 0; i < meta.out->sqld; i++ )
      {
         if ( i )
            wrapped_sql << ", ";

         wrapped_sql << ":" << meta.out->sqlvar[ i ].sqlname << suffix;
      }

      if ( meta.type == isc_info_sql_stmt_select )
         wrapped_sql << std::endl << "DO ";
      else
         wrapped_sql << ";" << std::endl;

      wrapped_sql << "SUSPEND";
   }

   wrapped_sql << ";" << std::endl << "END" << std::endl;

   free( meta.in );
   free( meta.out );

   delete [] index;

   *type = meta.type;

   return wrapped_sql.str();
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
