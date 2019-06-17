//-----------------------------------------------------------------------------

#ifndef DS_DB_CONNECTION_H
#define DS_DB_CONNECTION_H

//-----------------------------------------------------------------------------

#include <db/impl.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class connection : public impl
{
private:

   std::shared_ptr< impl > m_impl;

public:

   connection( impl * impl = nullptr );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name ) override;
   virtual void use( const std::string & name ) override;
   virtual bool drop( const std::string & name ) override;

   virtual statement operator()( const std::string & query,
                                 const name_list_t & parameters = {} ) override;

   virtual void begin_transaction( void ) override;
   virtual void commit_transaction( void ) override;
   virtual void rollback_transaction( void ) override;

   virtual void savepoint( const std::string & name ) override;
   virtual void release_savepoint( const std::string & name ) override;
   virtual void rollback_to_savepoint( const std::string & name ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
