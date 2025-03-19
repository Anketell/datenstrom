//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <mysql/positional_statement.h>
#include <mysql/rowset.h>
#include <mysql/error.h>

#include <sstream>
#include <iostream>

#include <cstring>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

positional_statement::positional_statement( MYSQL             & mysql,
                                            const std::string & sql ) :
statement_base( mysql )
{
   prepare( sql );
}

//-----------------------------------------------------------------------------

}
