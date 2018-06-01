//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <db/database.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

transaction::transaction( database & db ) :
m_db( db )
{
   m_db.begin_transaction();
}

//-----------------------------------------------------------------------------

transaction::~transaction( void )
{
   if ( std::uncaught_exception() )
      m_db.rollback_transaction();
   else
      m_db.commit_transaction();
}

//-----------------------------------------------------------------------------

savepoint::savepoint( database & db, const std::string & name ) :
m_db( db )
{
   m_name = name;
   m_db.savepoint( name.c_str() );
}

//-----------------------------------------------------------------------------

savepoint::~savepoint( void )
{
   if ( std::uncaught_exception() )
      m_db.rollback_to_savepoint( m_name.c_str() );
   else
      m_db.release_savepoint( m_name.c_str() );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
