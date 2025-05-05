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

   PQclear( result );

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
   if ( PQresultStatus( res ) != PGRES_COMMAND_OK )
   {
      std::string message = PQerrorMessage( conn );
      PQclear( res );
      throw_error( operation, message.c_str() );
   }
   PQclear( res );
}


//-----------------------------------------------------------------------------

}
