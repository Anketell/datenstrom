//-----------------------------------------------------------------------------

#include <mysql/statement.h>
#include <mysql/row.h>
#include <mysql/error.h>
#include <sstream>
#include <string.h>
#include <assert.h>

#include <iostream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

statement::statement( MYSQL                 & mysql,
                      const std::string     & sql,
                      const db::name_list_t & parameters ) :
m_mysql( mysql ),
m_query( sql ),
m_names( parameters )
{
}

//-----------------------------------------------------------------------------

statement::~statement( void )
{
}

//-----------------------------------------------------------------------------

const char * statement::check_parameter( int index )
{
   if ( m_state == Executed )
      reset();

   if ( index < 1 )
      throw std::runtime_error( "bad parameter" );

   if ( index > m_names.size() )
      throw std::runtime_error( "Too many parameters" );

   return m_names[ index - 1 ].c_str();
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int8_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << static_cast< int >( i ) << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int16_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int32_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, int64_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------


void statement::set_parameter( int index, uint8_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << static_cast< uint32_t > ( u  )<< ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint16_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint32_t u )
{
   m_values << "SET " << check_parameter( index ) << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, uint64_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, double d )
{
   m_values << "SET " << check_parameter( index )
            << " = " << d << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, const char * s, size_t length )
{
   std::string escaped;

   escaped.resize( length * 2 + 1 );
   length = mysql_real_escape_string( &m_mysql, const_cast< char * >( escaped.data() ), s, length );
   escaped.resize( length );

   m_values << "SET " << check_parameter( index )
            << " = \"" << escaped << "\";" << std::endl;
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, const char * s )
{
   set_parameter( index, s, strlen( s ) );
}

//-----------------------------------------------------------------------------

void statement::set_parameter( int index, const std::string & s )
{
   set_parameter( index, s.data(), s.length() );
}

//-----------------------------------------------------------------------------

int statement::parameter_count( void )
{
   return m_names.size();
}

//-----------------------------------------------------------------------------

void statement::reset( void )
{
   m_values.str().clear();

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

void statement::internal_execute( void )
{
   std::stringstream full_query;

   full_query << m_values.str() << m_query;

   for ( auto name : m_names )
      full_query << ";" << std::endl << "SET " << name << " = NULL";

   std::string query = full_query.str();

//   std::cout << query << std::endl << std::endl;

   int rc = mysql_real_query( &m_mysql, query.c_str(), query.length() );

   if ( rc )
      throw_error( "mysql_real_query failed", mysql_error( &m_mysql ) );

   m_state = Executed;
}

//-----------------------------------------------------------------------------

uint32_t statement::execute( void )
{
   internal_execute();

   uint32_t res = 0;

   MYSQL_RES * mysql_res = mysql_use_result( &m_mysql );
   if ( mysql_res )
   {
      for ( ;; )
      {
         MYSQL_FIELD * mysql_field = mysql_fetch_field( mysql_res );
         MYSQL_ROW     mysql_row   = mysql_fetch_row( mysql_res );

         switch ( mysql_field->type )
         {
            case MYSQL_TYPE_TINY :
               res = *reinterpret_cast< int8_t * >( *mysql_row );
               break;

            case MYSQL_TYPE_SHORT :
               res = *reinterpret_cast< int16_t * >( *mysql_row );
               break;

            case MYSQL_TYPE_LONG :
               res = *reinterpret_cast< int32_t * >( *mysql_row );
               break;
         }

         mysql_free_result( mysql_res );

         if ( mysql_next_result( &m_mysql ) )
            break;

         mysql_res = mysql_use_result( &m_mysql );
      }
   }
   else
      res = mysql_insert_id( &m_mysql );

   reset();

   return res;
}

//-----------------------------------------------------------------------------

db::row statement::result( void )
{
   internal_execute();

   return db::row( std::make_shared< row >( m_mysql ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}