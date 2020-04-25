//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_TRANSACTIONAL_H
#define DS_FIREBIRD_TRANSACTIONAL_H

//-----------------------------------------------------------------------------

#include <db/transactional.h>
#include <firebird/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

struct transactional : db::transactional
{
   isc_db_handle db_handle = 0;
   isc_tr_handle tr_handle = 0;

   virtual void begin_transaction( void ) override;
   virtual void commit_transaction( void ) override;
   virtual void rollback_transaction( void ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
