//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_SQLITE_TYPES_H
#define DS_SQLITE_TYPES_H

//-----------------------------------------------------------------------------

#include <sqlite3.h>
#include <sqlite/error.h>
#include <cassert>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

struct stmt_t
{
   enum state_t { Preparing, Executed };

   sqlite3_stmt * stmt   = nullptr;
   state_t        state  = Preparing;
   int            action = -1;

   void reset( void )
   {
      if ( state == Preparing )
         return;

      int rc = sqlite3_reset( stmt );
      if ( rc != SQLITE_OK )
         throw_error( "SQLite statement reset", rc );

      rc = sqlite3_clear_bindings( stmt );
      assert( rc == SQLITE_OK );

      state = stmt_t::Preparing;
   }

   ~stmt_t( void )
   {
      if ( stmt )
         sqlite3_finalize( stmt );

      stmt = nullptr;
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
