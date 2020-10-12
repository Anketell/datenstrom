//-----------------------------------------------------------------------------

#include <mssql/test/mssql_test_data.h>

//-----------------------------------------------------------------------------

const char * create = "CREATE TABLE Object ("
                      "i8 INTEGER,"
                      "i16 INTEGER,"
                      "i32 INTEGER,"
                      "i64 INTEGER,"
                      "u8 INTEGER,"
                      "u16 INTEGER,"
                      "u32 INTEGER,"
                      "u64 INTEGER,"
                      "f FLOAT,"
                      "d FLOAT,"
                      "hello VARCHAR( 10 ),"
                      "dt DATE,"
                      "id INTEGER NOT NULL PRIMARY KEY );";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "mssql://127.0.0.1:1433";
const char * bad_con_str  = "mssql:///blah";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, ?, NULL )";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( @i8, @i16, @i32, @i64, "
                                                    "@u8, @u16, @u32, @u64, "
                                                    "@float, @double, @string, "
                                                    "@date, NULL )";

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
