//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/guard.h>
#include <db/transaction.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

void guard( transactional & trans, guarded_fn fn )
{
   if ( !trans.tr_handle )
   {
      db::transaction transaction( trans );
      fn();
   }
   else
      fn();
}

//-----------------------------------------------------------------------------

}
