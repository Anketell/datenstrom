//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/types.h>
#include <mssql/error.h>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

void stmt_t::prepare_result_desc( void )
{
   static constexpr char operation[] = "MSSQL statement prepare result descriptions";

   if ( columns.size() )
      return;

   SQLSMALLINT count;

   RETCODE rc = SQLNumResultCols( hstmt, &count );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   columns.resize( count );

   for ( SQLSMALLINT i = 0; i < count; i++ )
   {
      stmt_t::desc_t & desc( columns[ i ] );

      rc = SQLDescribeCol( hstmt, i + 1, nullptr,
                           0,
                           nullptr,
                           &desc.type,
                           &desc.size,
                           &desc.digits,
                           &desc.nullable );

      check_status( operation, hstmt, SQL_HANDLE_STMT, rc );
   }
}

//-----------------------------------------------------------------------------

void stmt_t::execute( void )
{
   static constexpr char operation[] = "MSSQL statement execute";

   RETCODE rc = SQLExecute( hstmt );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   state = Executed;

   prepare_result_desc();
}

//-----------------------------------------------------------------------------

void stmt_t::reset( void )
{
   constexpr char operation[] = "MSSQL statement reset";

   if ( state == Preparing )
      return;

   RETCODE rc = SQLCancel( hstmt );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   rc = SQLFreeStmt( hstmt, SQL_UNBIND );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   rc = SQLFreeStmt( hstmt, SQL_RESET_PARAMS );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   rc = SQLFreeStmt( hstmt, SQL_CLOSE );
   check_status( operation, hstmt, SQL_HANDLE_STMT, rc );

   state = Preparing;
}

//-----------------------------------------------------------------------------

stmt_t::~stmt_t( void )
{
   if ( hstmt )
      SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

   hstmt = nullptr;
}

//-----------------------------------------------------------------------------

}
