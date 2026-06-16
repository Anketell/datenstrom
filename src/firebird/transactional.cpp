//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <firebird/transactional.h>
#include <firebird/error.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

void transactional::begin_transaction( void )
{
   if ( m_txn_count == 0 )
   {
      ISC_STATUS status[ status_vector_length ];

      isc_start_transaction( status, &tr_handle, 1, &db_handle, 0, nullptr );

      check_status( "Firebird begin transaction", status );
   }
   m_txn_count++;
}

//-----------------------------------------------------------------------------

void transactional::commit_transaction( void )
{
   if ( m_txn_count < 2 )
   {
      ISC_STATUS status[ status_vector_length ];

      isc_commit_transaction( status, &tr_handle );

      check_status( "Firebird commit transaction", status );
   }
   m_txn_count--;
}

//-----------------------------------------------------------------------------

void transactional::rollback_transaction( void )
{
   if ( m_txn_count < 2 )
   {
      ISC_STATUS status[ status_vector_length ];

      isc_rollback_transaction( status, &tr_handle );

      check_status( "Firebird rollback transaction", status );
   }
   m_txn_count--;
}

//-----------------------------------------------------------------------------

}
