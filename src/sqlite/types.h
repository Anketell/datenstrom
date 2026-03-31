//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once
//-----------------------------------------------------------------------------

#include <sqlite3.h>
#include <sqlite/error.h>
#include <cassert>

//-----------------------------------------------------------------------------

namespace ds::sqlite
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
      sqlite3_reset( stmt );
      int rc = sqlite3_clear_bindings( stmt );
      if ( rc != SQLITE_OK )
         throw_error( "SQLite clear bindings", rc );

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
