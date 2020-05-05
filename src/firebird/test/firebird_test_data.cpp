//-----------------------------------------------------------------------------

#include <firebird/test/firebird_test_data.h>

//-----------------------------------------------------------------------------

const char * create =
 R"(

SET TERM !! ;

CREATE GENERATOR ObjectID!!

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
   id INTEGER NOT NULL PRIMARY KEY )!!

SET TERM ; !!

)";

//-----------------------------------------------------------------------------

const char * simple_create =
 R"(

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
   id INTEGER NOT NULL PRIMARY KEY )

)";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "firebird://127.0.0.1:3050/tmp?username=sysdba&password=masterkey";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, "
                                                        "GEN_ID( ObjectID, 1 ) ) returning id";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( :i8, :i16, :i32, :i64, "
                                                    ":u8, :u16, :u32, :u64, "
                                                    ":float_, :double_, :string, "
                                                    "GEN_ID( ObjectID, 1 ) ) returning id";

const char * batch =
R"(

CREATE TABLE T1 (
   field INTEGER
);

CREATE TABLE T2 (
   field INTEGER
);

)";

//-----------------------------------------------------------------------------

Object data[] =
{
   { -8, -16, -32, -64, 8, 16, 32, 64, 12.34, 56.78, "Hello1" },
   { -16, -32, -64, -128, 16, 32, 64, 128, 24.68, 113.56, "Hello2" }
};

//-----------------------------------------------------------------------------
