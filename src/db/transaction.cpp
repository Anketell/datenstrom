//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <db/impl.h>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

transaction::transaction( transactional & db ) :
m_db( db )
{
   m_db.begin_transaction();

#ifdef __cpp_lib_uncaught_exceptions
   m_uncaught_count = std::uncaught_exceptions();
#endif
}

//-----------------------------------------------------------------------------

transaction::~transaction( void )
{
#ifdef __cpp_lib_uncaught_exceptions
   if ( std::uncaught_exceptions() > m_uncaught_count )
#else
   if ( std::uncaught_exception() )
#endif
      m_db.rollback_transaction();
   else
      m_db.commit_transaction();
}

//-----------------------------------------------------------------------------

savepoint::savepoint( impl & db, const std::string & name ) :
m_db( db )
{
   m_name = name;
   m_db.savepoint( name );

#ifdef __cpp_lib_uncaught_exceptions
   m_uncaught_count = std::uncaught_exceptions();
#endif
}

//-----------------------------------------------------------------------------

savepoint::~savepoint( void )
{
#ifdef __cpp_lib_uncaught_exceptions
   if ( std::uncaught_exceptions() > m_uncaught_count )
#else
   if ( std::uncaught_exception() )
#endif
      m_db.rollback_to_savepoint( m_name );
   else
      m_db.release_savepoint( m_name );
}

//-----------------------------------------------------------------------------

}
