//-----------------------------------------------------------------------------

#include <firebird/transactional.h>
#include <firebird/error.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

void transactional::begin_transaction( void )
{
   ISC_STATUS status[ status_vector_length ];

   isc_start_transaction( status, &tr_handle, 1, &db_handle, 0, nullptr );

   check_status( "Firebird begin transaction", status );
}

//-----------------------------------------------------------------------------

void transactional::commit_transaction( void )
{
   ISC_STATUS status[ status_vector_length ];

   isc_commit_transaction( status, &tr_handle );

   check_status( "Firebird commit transaction", status );
}

//-----------------------------------------------------------------------------

void transactional::rollback_transaction( void )
{
   ISC_STATUS status[ status_vector_length ];

   isc_rollback_transaction( status, &tr_handle );

   check_status( "Firebird rollback transaction", status );
}


//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
