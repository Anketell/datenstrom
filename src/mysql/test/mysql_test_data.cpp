//-----------------------------------------------------------------------------

#include <mysql/test/mysql_test_data.h>

//-----------------------------------------------------------------------------

const char * create =
R"(

set @@time_zone = '+00:00';

CREATE TABLE Object (
                      i8 INTEGER,
                      i16 INTEGER,
                      i32 INTEGER,
                      i64 INTEGER,
                      u8 INTEGER,
                      u16 INTEGER,
                      u32 INTEGER,
                      u64 INTEGER,
                      f DOUBLE,
                      d DOUBLE,
                      hello VARCHAR( 10 ),
                      dt DATE,
                      tm TIME,
                      dttm DATETIME,
                      id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY )
)";

//-----------------------------------------------------------------------------

const char * simple_create =
R"(

CREATE TABLE Object (
                      i8 INTEGER,
                      i16 INTEGER,
                      i32 INTEGER,
                      i64 INTEGER,
                      u8 INTEGER,
                      u16 INTEGER,
                      u32 INTEGER,
                      u64 INTEGER,
                      f DOUBLE,
                      d DOUBLE,
                      hello VARCHAR( 10 ),
                      dt DATE,
                      tm TIME,
                      dttm DATETIME,
                      id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY )
)";

//-----------------------------------------------------------------------------

const char * test_db_name = "test_db";
const char * test_con_str = "mysql://127.0.0.1:3306?username=root";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, ?, ?, "
                                                        "NULL )";
const char * insert_alt   = "INSERT INTO Object VALUES ( ?, ?, ?, ?, ?, ?, "
                                                        "?, ?, ?, ?, ?, "
                                                        "FROM_UNIXTIME( ?, '%Y-%m-%d' ), "
                                                        "FROM_UNIXTIME( ?, '%H:%i:%s' ), "
                                                        "FROM_UNIXTIME( ?, '%Y-%m-%d %H:%i:%s' ), NULL )";

const char * result      = "SELECT * FROM Object WHERE hello = ?";
const char * results     = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm "
                           "FROM Object ORDER BY hello";
const char * results_alt = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, "
                               "UNIX_TIMESTAMP( dt ), "
                               "TIME_TO_SEC( tm ), "
                               "UNIX_TIMESTAMP( dttm ) "
                           "FROM Object ORDER BY hello";

const char * num_rows = "SELECT COUNT( * ) FROM Object";
const char * del_rows = "DELETE FROM Object";
const char * named    = "INSERT INTO Object VALUES ( @i8, @i16, @i32, @i64, "
                                                    "@u8, @u16, @u32, @u64, "
                                                    "@float, @double, @string, "
                                                    "@date, @time, @datetime, NULL )";

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
