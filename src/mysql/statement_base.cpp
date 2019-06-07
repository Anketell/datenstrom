//-----------------------------------------------------------------------------

#include <mysql/statement_base.h>
#include <mysql/row.h>
#include <mysql/error.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

statement_base::statement_base( MYSQL & mysql, const std::string & sql )
{
   m_stmt = std::make_shared< stmt_t >();

   m_stmt->stmt = mysql_stmt_init( &mysql );

   int rc = mysql_stmt_prepare( m_stmt->stmt, sql.data(), sql.length() );
   if ( rc )
   {
      throw_error( "MySQL statement preparation",
                   mysql_stmt_error( m_stmt->stmt ) );
   }

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

void statement_base::internal_execute( void )
{
   static constexpr char operation[] = "MySQL statement execution";

   int rc = mysql_stmt_execute( m_stmt->stmt );
   if ( rc )
      throw_error( operation, mysql_stmt_error( m_stmt->stmt ) );

   m_state = Executed;
}

//-----------------------------------------------------------------------------

void statement_base::reset( void )
{
   int rc = mysql_stmt_reset( m_stmt->stmt );
   if ( rc )
      throw_error( "MySQL statement reset", mysql_stmt_error( m_stmt->stmt ) );

   m_state = Preparing;
}

//-----------------------------------------------------------------------------

uint32_t statement_base::execute( void )
{
   internal_execute();

   uint32_t res = 0;

   if ( mysql_stmt_num_rows( m_stmt->stmt ) )
   {
      try
      {
         row( m_stmt ).get_column( 0, res );
      }
      catch ( ... )
      {
         res = mysql_stmt_insert_id( m_stmt->stmt );
      }
   }
   else
      res = mysql_stmt_insert_id( m_stmt->stmt );

   reset();

   return res;
}

//-----------------------------------------------------------------------------

db::row statement_base::result( void )
{
   internal_execute();

   if ( mysql_stmt_affected_rows( m_stmt->stmt ) == 0 )
      return db::row();

   return db::row( std::make_shared< row >( m_stmt ) );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}