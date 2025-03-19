//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <windows.h>
#include <sql.h>
#include <string>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

[[noreturn]] void throw_error( const std::string & operation, const char * error );
void check_status( const char * operation, SQLHANDLE handle, SQLSMALLINT type, RETCODE rc );

//-----------------------------------------------------------------------------

}
