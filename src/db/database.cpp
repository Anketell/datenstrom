//-----------------------------------------------------------------------------

#include <db/database.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

database::database( std::shared_ptr< impl > impl ) :
m_impl( impl )
{
}

//-----------------------------------------------------------------------------

database & database::operator = ( impl * impl )
{
   m_impl.reset( impl );
   return *this;
}

//-----------------------------------------------------------------------------

database::operator impl * ( void ) const
{
   return m_impl.get();
}

//-----------------------------------------------------------------------------

const char * database::type( void ) const
{
   return m_impl->type();
}

//-----------------------------------------------------------------------------

void database::create( const std::string & name )
{
   m_impl->create( name );
}

//-----------------------------------------------------------------------------

void database::use( const std::string & name )
{
   m_impl->use( name );
}

//-----------------------------------------------------------------------------

bool database::drop( const std::string & name )
{
   return m_impl->drop( name );
}

//-----------------------------------------------------------------------------

statement database::operator()( const std::string & query,
                                const name_list_t & parameters )
{
   return ( *m_impl )( query, parameters );
}

//-----------------------------------------------------------------------------

void database::begin_transaction( void )
{
   m_impl->begin_transaction();
}

//-----------------------------------------------------------------------------

void database::commit_transaction( void )
{
   m_impl->commit_transaction();
}

//-----------------------------------------------------------------------------

void database::rollback_transaction( void )
{
   m_impl->rollback_transaction();
}

//-----------------------------------------------------------------------------

void database::savepoint( const std::string & name )
{
   m_impl->savepoint( name );
}

//-----------------------------------------------------------------------------

void database::release_savepoint( const std::string & name )
{
   m_impl->release_savepoint( name );
}

//-----------------------------------------------------------------------------

void database::rollback_to_savepoint( const std::string & name )
{
   m_impl->rollback_to_savepoint( name );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
