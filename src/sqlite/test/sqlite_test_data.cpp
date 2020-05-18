//-----------------------------------------------------------------------------

#include <sqlite/test/sqlite_test_data.h>

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
                      "dt2 DATE,"
                      "id INTEGER NOT NULL PRIMARY KEY );";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "sqlite:///tmp";
const char * bad_con_str  = "sqlite:///blah";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, ?, ?, NULL )";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, dt2 "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( @i8, @i16, @i32, @i64, "
                                                    "@u8, @u16, @u32, @u64, "
                                                    "@float, @double, @string, "
                                                    "@date, @unix_date, NULL )";

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
   { -8, -16, -32, -64, 8, 16, 32, 64, 12.34, 56.78, "Hello1", "2020-05-13", 3825 * 86400 },
   { -16, -32, -64, -128, 16, 32, 64, 128, 24.68, 113.56, "Hello2", "2020-05-14", 4825 * 86400 }
};

//-----------------------------------------------------------------------------
