//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/test/firebird_test_data.h>
#include <dsutil/filesys.h>

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

static const std::string test_con = "firebird://127.0.0.1:3050/" + ds::filesys::temp_directory() + "?username=sysdba&password=masterkey";

const char * test_con_str = test_con.c_str();
const char * test_db_name = "test_db";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, ?, ?, "
                                                        "GEN_ID( ObjectID, 1 ) ) returning id";
const char * insert_nulls = "INSERT INTO Object VALUES ( NULL, NULL, NULL, NULL, NULL, NULL, NULL, "
                                                        "NULL, NULL, NULL, NULL, NULL, NULL, NULL, "
                                                        "GEN_ID( ObjectID, 1 ) ) returning id";

const char * result   = "SELECT u64 FROM Object WHERE hello = ?";
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

const char * create_dom =
R"(
CREATE DOMAIN DM_ID AS CHAR( 1 );

CREATE TABLE Dom (
   id DM_ID )
)";

//-----------------------------------------------------------------------------

const char * insert_dom = "INSERT INTO Dom VALUES( :id )";

//-----------------------------------------------------------------------------
