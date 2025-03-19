//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <firebird/positional_statement.h>
#include <firebird/rowset.h>
#include <firebird/error.h>
#include <firebird/guard.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

positional_statement::positional_statement( transactional     & trans,
                                            const std::string & sql   ) :
statement_base( trans )
{
   prepare( sql );
}

//-----------------------------------------------------------------------------

}
