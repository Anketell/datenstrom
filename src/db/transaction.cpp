//-----------------------------------------------------------------------------

#include <db/transaction.h>
#include <db/impl.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

transaction::transaction( transactional & db ) :
m_db( db )
{
   m_db.begin_transaction();

#if __cplusplus >= 201703L
   m_count = std::uncaught_exceptions();
#endif
}

//-----------------------------------------------------------------------------

transaction::~transaction( void )
{
#if __cplusplus >= 201703L
   if ( std::uncaught_exceptions() > m_count )
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
   m_db.savepoint( name.c_str() );

#if __cplusplus >= 201703L
   m_count = std::uncaught_exceptions();
#endif
}

//-----------------------------------------------------------------------------

savepoint::~savepoint( void )
{
#if __cplusplus >= 201703L
   if ( std::uncaught_exceptions() > m_count )
#else
   if ( std::uncaught_exception() )
#endif
      m_db.rollback_to_savepoint( m_name.c_str() );
   else
      m_db.release_savepoint( m_name.c_str() );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
