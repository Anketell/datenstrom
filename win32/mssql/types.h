//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_TYPES_H
#define DS_MSSQL_TYPES_H

//-----------------------------------------------------------------------------

#include <windows.h>
#include <sql.h>
#include <vector>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

constexpr SQLSMALLINT sql_time_type = -9;

//-----------------------------------------------------------------------------

struct stmt_t
{
    struct desc_t
    {
        SQLSMALLINT type;
        SQLULEN     size;
        SQLSMALLINT digits;
        SQLSMALLINT nullable;
    };

   SQLHSTMT              hstmt = nullptr;
   std::vector< desc_t > columns;

   ~stmt_t( void )
   {
      if ( hstmt )
         SQLFreeHandle( SQL_HANDLE_STMT, hstmt );

      hstmt = nullptr;
   }
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

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
