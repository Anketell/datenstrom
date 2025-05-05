//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

struct pg_conn;
typedef struct pg_conn PGconn;

struct pg_result;
typedef struct pg_result PGresult;

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

enum data_types_t
{
   PG_BLOB     =   17,
   PG_SMALLINT =   21,
   PG_INTEGER  =   23,
   PG_BIGINT   =   20,
   PG_FLOAT    =  700,
   PG_DOUBLE   =  701,
   PG_TEXT     = 1043,
   PG_DATE     = 1082,
   PG_TIME     = 1083,
   PG_DATETIME = 1114
};

//-----------------------------------------------------------------------------

struct stmt_t
{
   enum state_t { Preparing, Executed };

   PGconn    * conn;
   PGresult  * result;
   std::string name;
   state_t     state  = Preparing;

   void reset( void );

   ~stmt_t( void );
};

//-----------------------------------------------------------------------------

}
