//-----------------------------------------------------------------------------

#include <firebird/test/firebird_test_data.h>

//-----------------------------------------------------------------------------

const char * create =
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
   id INTEGER NOT NULL PRIMARY KEY );

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
   dt DATE,
   tm TIME,
   dttm TIMESTAMP,
   id INTEGER NOT NULL PRIMARY KEY )

)";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "firebird://127.0.0.1:3050/tmp?username=sysdba&password=masterkey";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, ?, ?, "
                                                        "GEN_ID( ObjectID, 1 ) ) returning id";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( :i8, :i16, :i32, :i64, "
                                                    ":u8, :u16, :u32, :u64, "
                                                    ":float_, :double_, :string, "
                                                    ":date_, :time_, :datetime_, "
                                                    "GEN_ID( ObjectID, 1 ) ) returning id";
const char * named_duplicates = "INSERT INTO Object VALUES ( :signed, :signed, :signed, :signed, "
                                                            ":unsigned, :unsigned, :unsigned, :unsigned, "
                                                            ":float_, :double_, :string, "
                                                            ":date_, :time_, :datetime_, "
                                                            "GEN_ID( ObjectID, 1 ) ) returning id";


const char * batch =
R"(

SET TERM !! ;

CREATE TABLE T1 (
   field INTEGER
)!!

CREATE TABLE T2 (
   field INTEGER
)!!

SET TERM ; !!

)";

//-----------------------------------------------------------------------------
