//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <postgresql/statement_base.h>
#include <postgresql/rowset.h>
#include <postgresql/error.h>
#include <dsutil/endian.h>
#include <libpq-fe.h>
#include <charconv>
#include <cstring>
#include <limits>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

statement_base::statement_base( PGconn * conn )
{
   char buffer[ 9 ] = {};

   m_stmt = std::make_shared< stmt_t >();
   m_stmt->conn = conn;

   std::to_chars( buffer,
                  buffer + sizeof( buffer ),
                  reinterpret_cast< uint64_t >( m_stmt.get() ), 16 );

   m_name  = "DS";
   m_name += buffer;
}

//-----------------------------------------------------------------------------

statement_base::~statement_base( void )
{
}

//-----------------------------------------------------------------------------

void statement_base::prepare( const std::string & sql )
{
   constexpr char operation[] = "PostgreSQL statement prepare";

   PGresult * res = PQprepare( m_stmt->conn,
                               m_name.c_str(),
                               sql.c_str(),
                               0, nullptr );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      char * error = PQresultVerboseErrorMessage( res, PQERRORS_DEFAULT, PQSHOW_CONTEXT_ALWAYS );
      std::string message = error;
      PQfreemem( error );
      PQclear( res );
      m_stmt->name.clear();
      throw_error( operation, message.c_str() );
   }
   PQclear( res );

   m_stmt->name = m_name;

   res = PQdescribePrepared( m_stmt->conn, m_stmt->name.c_str() );

   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::string message = PQerrorMessage( m_stmt->conn );
      PQclear( res );
      throw_error( operation, message.c_str() );
   }

   m_param_count = PQnparams( res );
   m_param_types.reserve( m_param_count );

   for ( int i = 0; i < m_param_count; i++ )
      m_param_types.push_back( PQparamtype( res, i ) );

   PQclear( res );

   m_param_values.resize( m_param_count );
   m_param_data.resize( m_param_count );
   m_param_lengths.resize( m_param_count );
   m_param_formats.resize( m_param_count );
}

//-----------------------------------------------------------------------------

void statement_base::check_parameter( int index )
{
   static constexpr char operation[] = "PostgreSQL statement parameter check";

   if ( index < 0 )
      throw_error( operation, "Bad parameter" );

   if ( index >= m_param_count )
      throw_error( operation, "Too many parameters" );
}

//-----------------------------------------------------------------------------

void statement_base::internal_execute( void )
{
   constexpr char operation[] = "PostgreSQL statement execute";

   m_stmt->result = PQexecPrepared( m_stmt->conn,
                                    m_stmt->name.c_str(),
                                    m_param_count,
                                    m_param_values.data(),
                                    m_param_lengths.data(),
                                    m_param_formats.data(), 1 );

   int status = PQresultStatus( m_stmt->result );
   if ( status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK)
   {
      char * error = PQresultVerboseErrorMessage( m_stmt->result, PQERRORS_DEFAULT, PQSHOW_CONTEXT_ALWAYS );
      std::string message = error;
      PQfreemem( error );
      PQclear( m_stmt->result );
      throw_error( operation, message.c_str() );
   }

   m_stmt->state = stmt_t::Executed;
}

//-----------------------------------------------------------------------------

template< typename BI > void statement_base::set_big_int( int index, BI bi )
{
   constexpr char operation[] = "PostgreSQL statement bind integer";

   check_parameter( index );

   typedef std::numeric_limits< BI > limits;

   typedef typename std::conditional< limits::is_signed, int16_t, uint16_t >::type SI;
   typedef typename std::conditional< limits::is_signed, int32_t, uint32_t >::type LI;

   int type = m_param_types[ index ];

   switch ( type )
   {
      case PG_SMALLINT:
      {
         typedef std::numeric_limits< SI > limits;

         if ( bi < limits::lowest() || bi > limits::max() )
            throw_error( operation, "Out of numeric range of SMALLINT" );

         reinterpret_cast< uint16_t & >( m_param_data[ index ] ) = ds::endian::htob( static_cast< uint16_t >( bi ) );
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( int16_t );
         m_param_formats[ index ] = 1;

         break;
      }

      case PG_INTEGER:
      {
         typedef std::numeric_limits< LI > limits;

         if ( bi < limits::lowest() || bi > limits::max() )
            throw_error( operation, "Out of numeric range of INTEGER" );

         reinterpret_cast< uint32_t & >( m_param_data[ index ] ) = ds::endian::htob( static_cast< uint32_t >( bi ) );
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( int32_t );
         m_param_formats[ index ] = 1;

         break;
      }

      case PG_BIGINT:
         m_param_data[ index ]    = static_cast< int64_t >( ds::endian::htob( static_cast< uint64_t >( bi ) ) );
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( int64_t );
         m_param_formats[ index ] = 1;
         break;

      case PG_FLOAT:
      {
         float    f = static_cast< float >( bi );
         uint32_t u = ds::endian::htob( *reinterpret_cast< uint32_t * >( &f ) );
         reinterpret_cast< uint32_t & >( m_param_data[ index ] ) = u;
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( float );
         m_param_formats[ index ] = 1;
         break;
      }

      case PG_DOUBLE:
      {
         double   d = static_cast< double >( bi );
         uint64_t u = ds::endian::htob( *reinterpret_cast< uint64_t * >( &d ) );
         reinterpret_cast< double & >( m_param_data[ index ] ) = static_cast< double >( bi );
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( double );
         m_param_formats[ index ] = 1;
         break;
      }

      default:
         throw_error( operation, "Not numeric type" );
   }
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int8_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int16_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int32_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, int64_t i )
{
   set_big_int< int64_t >( index, i );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint8_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint16_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint32_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, uint64_t u )
{
   set_big_int< uint64_t >( index, u );
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, double d )
{
   constexpr char operation[] = "PostgreSQL statement floating point";

   check_parameter( index );

   int type = m_param_types[ index ];

   switch ( type )
   {
      case PG_FLOAT:
      {
         float    f = static_cast< float >( d );
         uint32_t u = ds::endian::htob( *reinterpret_cast< uint32_t * >( &f ) );
         reinterpret_cast< uint32_t & >( m_param_data[ index ] ) = u;
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( float );
         m_param_formats[ index ] = 1;
         break;
      }

      case PG_DOUBLE:
      {
         uint64_t u = ds::endian::htob(  *reinterpret_cast< uint64_t * >( &d ) );
         reinterpret_cast< uint64_t & >( m_param_data[ index ] ) = u;
         m_param_values[ index ]  = reinterpret_cast< char * >( &m_param_data[ index ] );
         m_param_lengths[ index ] = sizeof( double );
         m_param_formats[ index ] = 1;
         break;
      }

      default:
         throw_error( operation, "Not floating point type" );
   }
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const char * s )
{
   check_parameter( index );

   m_param_values[ index ]  = const_cast< char * >( s );
   m_param_lengths[ index ] = strlen( s );
   m_param_formats[ index ] = m_param_types[ index ] == PG_BLOB;
}

//-----------------------------------------------------------------------------

void statement_base::set_parameter( int index, const std::string & s )
{
   check_parameter( index );

   m_param_values[ index ]  = const_cast< char * >( s.data() );
   m_param_lengths[ index ] = s.length();
   m_param_formats[ index ] = m_param_types[ index ] == PG_BLOB;
}

//-----------------------------------------------------------------------------

int statement_base::parameter_count( void )
{
   return m_param_count;
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   m_stmt->reset();
}

//-----------------------------------------------------------------------------

void statement_base::execute( void )
{
   internal_execute();
   reset();
}

//-----------------------------------------------------------------------------

db::rowset statement_base::result( void )
{
   internal_execute();
   return db::rowset( std::make_shared< pg::rowset >( m_stmt ) );
}

//-----------------------------------------------------------------------------

}
