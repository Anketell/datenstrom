//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/transactional.h>
#include <firebird/types.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

struct transactional : db::transactional
{
   isc_db_handle db_handle   = 0;
   isc_tr_handle tr_handle   = 0;
   int           m_txn_count = 0;


   virtual void begin_transaction( void ) override;
   virtual void commit_transaction( void ) override;
   virtual void rollback_transaction( void ) override;
};

//-----------------------------------------------------------------------------

}
