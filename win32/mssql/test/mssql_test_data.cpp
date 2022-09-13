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
const char * test_con_str   = "mssql://127.0.0.1:1433";
const char * tester_con_str = "mssql://127.0.0.1:1433?username=tester&password=tester";
const char * azure_con_str  = "mssql://pryda-pm-analytics.database.windows.net:1433?username=PMA_admin&password=Gumpti0n";
const char * bad_con_str    = "mssql:///blah";
const char * bad_sql        = "THIS IS BAD SQL";
const char * insert = "INSERT INTO Object ( i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm ) "
                                               "OUTPUT INSERTED.ID "
                                               "VALUES ( ?, ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ? ,? ,? )";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
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

CREATE TABLE T1 (
   field INTEGER
);

CREATE TABLE T2 (
   field INTEGER
);

CREATE FUNCTION F1()
RETURNS INT
AS
BEGIN
   RETURN 123456
END;

CREATE FUNCTION F2()
RETURNS INT
AS
BEGIN
   RETURN 234567
END;

)";

//-----------------------------------------------------------------------------
