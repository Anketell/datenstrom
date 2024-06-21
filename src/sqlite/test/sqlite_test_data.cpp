//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/test/sqlite_test_data.h>
#include <dsutil/filesys.h>

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
                      "tm DATETIME,"
                      "dttm DATETIME,"
                      "id INTEGER NOT NULL PRIMARY KEY );";

//-----------------------------------------------------------------------------

static const std::string test_con = "sqlite:///" + ds::filesys::temp_directory();

const char * test_con_str = test_con.c_str();
const char * test_db_name = "test_db";
const char * bad_con_str  = "sqlite:///blah";
const char * bad_sql      = "THIS IS BAD SQL";
const char * insert       = "INSERT INTO Object( i8, i16, i32, i64, "
                                                "u8, u16, u32, u64, "
                                                "f, d, hello, dt, tm, dttm ) "
                            "VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )";
const char * insert_id    = "INSERT INTO Object( i8, i16, i32, i64, "
                                                "u8, u16, u32, u64, "
                                                "f, d, hello, dt, tm, dttm ) "
                             "VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? ) "
                             "RETURNING id;";
const char * result   = "SELECT * FROM Object WHERE hello = ?";
const char * results  = "SELECT i8, i16, i32, i64, u8, u16, u32, u64, f, d, hello, dt, tm, dttm "
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
