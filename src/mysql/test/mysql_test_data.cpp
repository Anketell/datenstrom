//-----------------------------------------------------------------------------

#include <mysql/test/mysql_test_data.h>

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
                      "f DOUBLE,"
                      "d DOUBLE,"
                      "hello VARCHAR( 10 ),"
                      "id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY )";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "mysql://127.0.0.1?username=root";
const char * bad_db_name  = "/blah/test.db";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, NULL )";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello "
                          "FROM Object ORDER BY hello";
const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( @i8, @i16, @i32, @i64, "
                                                "@u8, @u16, @u32, @u64, "
                                                "@float, @double, @string, NULL )";

//-----------------------------------------------------------------------------

Object data[] =
{
   { -8, -16, -32, -64, 8, 16, 32, 64, 12.34, 56.78, "Hello1" },
   { -16, -32, -64, -128, 16, 32, 64, 128, 24.68, 113.56, "Hello2" }
};

//-----------------------------------------------------------------------------
