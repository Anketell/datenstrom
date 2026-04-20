//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

class transactional
{
   int m_txn_count = 0;

public:

   virtual void begin_transaction( void );
   virtual void commit_transaction( void );
   virtual void rollback_transaction( void );

   bool in_transaction( void ) const;
};

//-----------------------------------------------------------------------------

}
