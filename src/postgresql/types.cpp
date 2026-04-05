//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <postgresql/types.h>
#include <postgresql/error.h>
#include <libpq-fe.h>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

void stmt_t::reset( void )
{
   if ( state == Preparing )
      return;

   if ( result )
   {
      PQclear( result );
      result = nullptr;
   }

   state = Preparing;
}

//-----------------------------------------------------------------------------

stmt_t::~stmt_t( void )
{
   constexpr char operation[] = "PostgreSQL deallocate statement";

   reset();

   if ( name.empty() )
      return;

   PGresult * res = PQexec( conn, ( "DEALLOCATE \"" + name + "\"" ).c_str() );
   PQclear( res );
}


//-----------------------------------------------------------------------------

}
