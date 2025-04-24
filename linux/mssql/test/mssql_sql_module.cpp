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
   SELECT 0;
)"
},
   
//-----------------------------------------------------------------------------

{ "test.batch",

R"(
------------------------------------------------

CREATE FUNCTION F1()
RETURNS INT
AS
BEGIN
   RETURN 123456
END;

---------------------------------

CREATE FUNCTION F2()
RETURNS INT
AS
BEGIN
   RETURN 234567
END;

---------------------------------

CREATE TABLE T1 (
   field INTEGER
);

---------------------------------

INSERT INTO T1 VALUES ( 47 );

---------------------------------

CREATE TABLE T2 (
   field INTEGER
);

---------------------------------

INSERT INTO T2 VALUES ( 23 );

---------------------------------
/*
CREATE TABLE T2 (
   field INTEGER
);
*/

---------------------------------

DROP FUNCTION F1;

---------------------------------

CREATE FUNCTION F1()
RETURNS INT
AS
BEGIN
   RETURN 654321;
END;

---------------------------------

DROP FUNCTION F2;

---------------------------------

CREATE FUNCTION F2()
RETURNS VARCHAR( 32 )
AS
BEGIN
   RETURN '765432;'
END;

---------------------------------

CREATE VIEW T1_V
AS
   SELECT field FROM T1;

---------------------------------

CREATE VIEW T2_V
AS
   SELECT field FROM T2;

---------------------------------
)"
},

//-----------------------------------------------------------------------------

{ "test.create",

R"(
    CREATE TABLE Object
    (
        i8           SMALLINT,
        i16          SMALLINT,
        i32          INT,
        i64          BIGINT,
        u8           TINYINT,
        u16          SMALLINT,
        u32          INT,
        u64          BIGINT,
        f            FLOAT,
        d            FLOAT( 53 ),
        hello        VARCHAR( 10 ),
        dt           DATE,
        tm           TIME,
        dttm         DATETIME2,
        id           INT IDENTITY( 1, 1 ) PRIMARY KEY
   )
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
      dttm 
   )      
   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )
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
      dttm 
   )      
   OUTPUT INSERTED.ID
   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )
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
         dttm
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
         $datetime
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
      data VARBINARY( 8000 ) )
   )"
   },
   
   //-----------------------------------------------------------------------------
   
   { "test.insert_blob",
   
   "INSERT INTO Object VALUES( ? )"
   },
      
   //-----------------------------------------------------------------------------
   
   { "test.select_blob",
   
   "SELECT * FROM Object"
   },
      
   //-----------------------------------------------------------------------------

{ "test.type",

"mssql"

}

};

//-----------------------------------------------------------------------------

const char module_type[] = "mssql";

//-----------------------------------------------------------------------------

}
