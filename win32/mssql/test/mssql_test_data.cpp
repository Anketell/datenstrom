//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mssql/test/mssql_test_data.h>

//-----------------------------------------------------------------------------

const char * create =
R"(

CREATE TABLE Object(
                      i8 SMALLINT,
                      i16 SMALLINT,
                      i32 INT,
                      i64 BIGINT,
                      u8 TINYINT,
                      u16 SMALLINT,
                      u32 INT,
                      u64 BIGINT,
                      f FLOAT,
                      d FLOAT( 53 ),
                      hello VARCHAR( 10 ),
                      dt DATE,
                      tm TIME,
                      dttm DATETIME2,
                      id INT IDENTITY( 1, 1 ) PRIMARY KEY
)
)";

//-----------------------------------------------------------------------------

const char * create_add2 =
R"(
CREATE FUNCTION ADD2( @value1 INT, @value2 INT )
RETURNS INT
AS
BEGIN
   RETURN ( @value1 + @value2 );
END;
)";

//-----------------------------------------------------------------------------

const char * test_db_name   = "test_db";
const char * test_con_str   = MSSQL_TEST;
const char * tester_con_str = MSSQL_TEST"?username=tester&password=tester";
const char * azure_con_str  = "mssql://pryda-pm-analytics.database.windows.net:1433?username=PMA_admin&password=Gumpti0n";
const char * bad_con_str    = "mssql:///blah";
const char * bad_sql        = "THIS IS BAD SQL";
const char * insert = "INSERT INTO Object ( i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm ) "
                                               "OUTPUT INSERTED.ID "
                                               "VALUES ( ?, ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ? ,? ,? )";
const char * result   = "SELECT u64 FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( $i8, $i16, $i32, $i64, "
                                                    "$u8, $u16, $u32, $u64, "
                                                    "$float, $double, $string, "
                                                    "$date, $time, $datetime )";
const char * named_duplicates = "INSERT INTO Object VALUES ( $signed, $signed, $signed, $signed, "
                                                            "$unsigned, $unsigned, $unsigned, $unsigned, "
                                                            "$float, $double, $string, "
                                                            "$date, $time, $datetime )";

const char * batch =
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
)";

//-----------------------------------------------------------------------------
