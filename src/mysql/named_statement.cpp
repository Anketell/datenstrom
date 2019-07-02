//-----------------------------------------------------------------------------

#include <mysql/named_statement.h>
#include <mysql/result.h>
#include <mysql/error.h>
#include <sstream>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

named_statement::named_statement( MYSQL                 & mysql,
                                  const std::string     & sql,
                                  const db::name_list_t & parameters ) :
statement_base( mysql, sql ),
m_mysql( mysql ),
m_names( parameters )
{
}

//-----------------------------------------------------------------------------

named_statement::~named_statement( void )
{
}

//-----------------------------------------------------------------------------

const char * named_statement::check_parameter( int index )
{
   static constexpr char operation[] = "MySQL named statement parameter check";

   if ( m_state == Executed )
      reset();

   if ( index < 1 )
      throw_error( operation, "Bad parameter" );

   if ( index > m_names.size() )
      throw_error( operation, "Too many parameters" );

   return m_names[ index - 1 ].c_str();
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int8_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << static_cast< int >( i ) << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int16_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int32_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, int64_t i )
{
   m_values << "SET " << check_parameter( index )
            << " = " << i << ";" << std::endl;
}

//-----------------------------------------------------------------------------


void named_statement::set_parameter( int index, uint8_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << static_cast< uint32_t > ( u  )<< ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint16_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint32_t u )
{
   m_values << "SET " << check_parameter( index ) << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, uint64_t u )
{
   m_values << "SET " << check_parameter( index )
            << " = " << u << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, double d )
{
   m_values << "SET " << check_parameter( index )
            << " = " << d << ";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, const char * s, size_t length )
{
   std::string escaped;

   escaped.resize( length * 2 + 1 );
   length = mysql_real_escape_string( &m_mysql, const_cast< char * >( escaped.data() ), s, length );
   escaped.resize( length );

   m_values << "SET " << check_parameter( index )
            << " = \"" << escaped << "\";" << std::endl;
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, const char * s )
{
   set_parameter( index, s, strlen( s ) );
}

//-----------------------------------------------------------------------------

void named_statement::set_parameter( int index, const std::string & s )
{
   set_parameter( index, s.data(), s.length() );
}

//-----------------------------------------------------------------------------

int named_statement::parameter_count( void )
{
   return m_names.size();
}

//-----------------------------------------------------------------------------

void named_statement::reset( void )
{
   statement_base::reset();

   m_values.str().clear();
}

//-----------------------------------------------------------------------------

void named_statement::internal_execute( void )
{
   static constexpr char operation[] = "MySQL named statement parameter binding";

   std::string values = m_values.str();

   int rc = mysql_real_query( &m_mysql, values.c_str(), values.length() );
   if ( rc )
      throw_error( operation, mysql_error( &m_mysql ) );

   do
   {
      rc = mysql_next_result( &m_mysql );
      if ( rc > 0 )
         throw_error( operation, mysql_error( &m_mysql ) );
   }
   while ( rc == 0 );

   statement_base::internal_execute();
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}