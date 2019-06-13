//-----------------------------------------------------------------------------

#include <db/connection.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

connection::connection( std::shared_ptr< impl > impl ) :
m_impl( impl )
{
}

//-----------------------------------------------------------------------------

connection & connection::operator = ( impl * impl )
{
   m_impl.reset( impl );
   return *this;
}

//-----------------------------------------------------------------------------

connection::operator impl * ( void ) const
{
   return m_impl.get();
}

//-----------------------------------------------------------------------------

const char * connection::type( void ) const
{
   return m_impl->type();
}

//-----------------------------------------------------------------------------

void connection::create( const std::string & name )
{
   m_impl->create( name );
}

//-----------------------------------------------------------------------------

void connection::use( const std::string & name )
{
   m_impl->use( name );
}

//-----------------------------------------------------------------------------

bool connection::drop( const std::string & name )
{
   return m_impl->drop( name );
}

//-----------------------------------------------------------------------------

statement connection::operator()( const std::string & query,
                                const name_list_t & parameters )
{
   return ( *m_impl )( query, parameters );
}

//-----------------------------------------------------------------------------

void connection::begin_transaction( void )
{
   m_impl->begin_transaction();
}

//-----------------------------------------------------------------------------

void connection::commit_transaction( void )
{
   m_impl->commit_transaction();
}

//-----------------------------------------------------------------------------

void connection::rollback_transaction( void )
{
   m_impl->rollback_transaction();
}

//-----------------------------------------------------------------------------

void connection::savepoint( const std::string & name )
{
   m_impl->savepoint( name );
}

//-----------------------------------------------------------------------------

void connection::release_savepoint( const std::string & name )
{
   m_impl->release_savepoint( name );
}

//-----------------------------------------------------------------------------

void connection::rollback_to_savepoint( const std::string & name )
{
   m_impl->rollback_to_savepoint( name );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
