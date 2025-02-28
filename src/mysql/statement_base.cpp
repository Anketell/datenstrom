//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mysql/statement_base.h>
#include <mysql/rowset.h>
#include <mysql/error.h>
#include <db/simple_rowset.h>

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

void statement_base::execute( void )
{
   static constexpr char operation[] = "MySQL statement execute";

   internal_execute();
   reset();
}

//-----------------------------------------------------------------------------

db::rowset statement_base::result( void )
{
   internal_execute();

   db::rowset result( std::make_shared< mysql::rowset >( m_stmt ) );

   if ( result.eof() )
   {
      uint64_t value = mysql_stmt_insert_id( m_stmt->stmt );
      if ( value )
         result = db::rowset( std::make_shared< db::simple_rowset >( value ) );
   }
      

   return result;
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}