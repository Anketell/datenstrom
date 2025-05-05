//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <sql.h>
#include <vector>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

constexpr SQLSMALLINT sql_datetime_type_1 = -154;
constexpr SQLSMALLINT sql_datetime_type_2 = 91;
constexpr SQLSMALLINT sql_datetime_type_3 = 93;

//-----------------------------------------------------------------------------

struct stmt_t
{
   enum state_t { Preparing, Executed };

   struct desc_t
   {
      SQLSMALLINT type;
      SQLULEN     size;
      SQLSMALLINT digits;
      SQLSMALLINT nullable;
      SQLLEN      ind_len;
   };

   SQLHSTMT              hstmt = nullptr;
   std::vector< desc_t > columns;
   state_t               state = Preparing;

   void execute( void );
   void prepare_result_desc( void );
   void reset( void );

   ~stmt_t( void );
};

//-----------------------------------------------------------------------------

inline SQLCHAR * sql_char( const char * sql )
{
   return const_cast< SQLCHAR * >( reinterpret_cast< const SQLCHAR * >( sql ) );
}

//-----------------------------------------------------------------------------

inline SQLSMALLINT sql_smint( size_t size )
{
    return static_cast< SQLSMALLINT >( size );
}

//-----------------------------------------------------------------------------

inline SQLINTEGER sql_int( size_t size )
{
    return static_cast< SQLINTEGER >( size );
}

//-----------------------------------------------------------------------------

}
