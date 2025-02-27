//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <sqlite/test/sqlite_test_data.h>
#include <dsutil/filesys.h>

//-----------------------------------------------------------------------------

static const std::string test_con = "sqlite:///" + ds::filesys::temp_directory();

const char * test_con_str = test_con.c_str();
const char * test_db_name = "test_db";
const char * bad_con_str  = "sqlite:///blah";
const char * bad_sql      = "THIS IS BAD SQL";

//-----------------------------------------------------------------------------
