//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/sql_module_harness.h>

//-----------------------------------------------------------------------------

namespace ds::db::sql
{

//-----------------------------------------------------------------------------

const sql_map_t sql_map =
{

//-----------------------------------------------------------------------------

{ "test.simple",

R"(
   CREATE TABLE T1 (
      field INTEGER
   )
)"
},
   
//-----------------------------------------------------------------------------

{ "test.create",

R"(
   CREATE GENERATOR ObjectID;

   CREATE TABLE Object (
      i8 SMALLINT,
      i16 SMALLINT,
      i32 INTEGER,
      i64 BIGINT,
      u8 SMALLINT,
      u16 SMALLINT,
      u32 INTEGER,
      u64 BIGINT,
      f FLOAT,
      d DOUBLE PRECISION,
      hello VARCHAR( 10 ),
      dt DATE,
      tm TIME,
      dttm TIMESTAMP,
      ob_id INTEGER NOT NULL PRIMARY KEY );
)"
},

//-----------------------------------------------------------------------------

{ "test.batch",

R"(
   CREATE TABLE T1 (
      field INTEGER
   );
   
   CREATE TABLE T2 (
      field INTEGER
   );
)"
},

//-----------------------------------------------------------------------------

{ "test.insert",

R"(
   INSERT INTO Object
   (
      i8, 
      i16, 
      i32, 
      i64, 
      u8, 
      u16, 
      u32, 
      u64, 
      f, 
      d, 
      hello, 
      dt, 
      tm, 
      dttm, 
      ob_id
   )      
   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GEN_ID( ObjectID, 1 ) )
)"
},

//-----------------------------------------------------------------------------

{ "test.insert_null",

R"(
   INSERT INTO Object
   (
      i8, 
      i16, 
      i32, 
      i64, 
      u8, 
      u16, 
      u32, 
      u64, 
      f, 
      d, 
      hello, 
      dt, 
      tm, 
      dttm,
      ob_id 
   )      
   VALUES ( NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, GEN_ID( ObjectID, 1 ) )
)"
},

//-----------------------------------------------------------------------------

{ "test.insert_id",

R"(
   INSERT INTO Object
   (
      i8, 
      i16, 
      i32, 
      i64, 
      u8, 
      u16, 
      u32, 
      u64, 
      f, 
      d, 
      hello, 
      dt, 
      tm, 
      dttm, 
      ob_id
   )      
   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, GEN_ID( ObjectID, 1 ) )
   RETURNING ob_id;
)"
},

//-----------------------------------------------------------------------------

{ "test.result",

R"(
   SELECT u64 FROM Object WHERE hello = ?
)"
},

//-----------------------------------------------------------------------------

{ "test.no_result",

R"(
   CREATE TABLE T1 (
      field INTEGER
)
)"
},

//-----------------------------------------------------------------------------

{ "test.results",

R"(
   SELECT i8, 
          i16, 
          i32, 
          i64, 
          u8, 
          u16, 
          u32, 
          u64, 
          f, 
          d, 
          hello, 
          dt, 
          tm, 
          dttm
      FROM Object ORDER BY hello
)"
},

//-----------------------------------------------------------------------------

{ "test.num_rows",

R"(
   SELECT COUNT( * ) FROM Object
)"
},

//-----------------------------------------------------------------------------

{ "test.del_rows",

R"(
   DELETE FROM Object
)"
},

//-----------------------------------------------------------------------------

{ "test.named",

R"(
   INSERT INTO Object 
   (
      i8, 
      i16, 
      i32, 
      i64, 
      u8, 
      u16, 
      u32, 
      u64, 
      f, 
      d, 
      hello, 
      dt, 
      tm, 
      dttm,
      ob_id
   )
   VALUES 
   ( 
      $i8, 
      $i16, 
      $i32, 
      $i64,
      $u8, 
      $u16, 
      $u32, 
      $u64, 
      $float, 
      $double, 
      $string,
      $date, 
      $time, 
      $datetime,
      GEN_ID( ObjectID, 1 )
   )
)"
},

//-----------------------------------------------------------------------------

{ "test.bad_sql",

R"(
   THIS IS BAD SQL
)"
},

//-----------------------------------------------------------------------------

{ "test.create_blob",

R"(
   CREATE TABLE Object (
   data BLOB )
)"
},

//-----------------------------------------------------------------------------

{ "test.insert_blob",

"INSERT INTO Object VALUES( ? )"
},
   
//-----------------------------------------------------------------------------

{ "test.insert_null_blob",

"INSERT INTO Object VALUES( NULL )"
},
   
//-----------------------------------------------------------------------------

{ "test.select_blob",

"SELECT * FROM Object"
},
   
//-----------------------------------------------------------------------------

{ "test.create_index",

"CREATE UNIQUE INDEX object_hello_index ON Object( Hello )"
},
   
//-----------------------------------------------------------------------------

{ "test.type",

"firebird"
},
};

//-----------------------------------------------------------------------------

const char module_type[] = "firebird";

//-----------------------------------------------------------------------------

}
