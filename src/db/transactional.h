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
public:

   virtual void begin_transaction( void )    = 0;
   virtual void commit_transaction( void )   = 0;
   virtual void rollback_transaction( void ) = 0;
};

//-----------------------------------------------------------------------------

}
