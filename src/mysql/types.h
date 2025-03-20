//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <mysql.h>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

struct bind_info_t
{
   unsigned long length;
   my_bool       is_null;
   my_bool       error;
};

//-----------------------------------------------------------------------------

struct stmt_t
{
   MYSQL_STMT  * stmt       = nullptr;
   MYSQL_BIND  * mysql_bind = nullptr;
   bind_info_t * bind_info  = nullptr;
   int           count      = 0;

   ~stmt_t( void )
   {
      if ( stmt )
         mysql_stmt_close( stmt );

      if ( mysql_bind )
      {
         for ( int i = 0; i < count; i++ )
            free( mysql_bind[ i ].buffer );

         delete [] mysql_bind;
         delete [] bind_info;
      }
   }
};

//-----------------------------------------------------------------------------

}
