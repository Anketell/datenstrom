//-----------------------------------------------------------------------------

#ifndef DS_DB_DATABASE_H
#define DS_DB_DATABASE_H

//-----------------------------------------------------------------------------

#include <db/statement.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class database
{
protected:

   virtual ~database( void ) {}

public:

   virtual statement operator()( const std::string & query,
                                 const name_list_t & parameters = {} ) = 0;

   virtual void begin_transaction( void ) = 0;
   virtual void commit_transaction( void ) = 0;
   virtual void rollback_transaction( void ) = 0;

   virtual void savepoint( const std::string & name ) = 0;
   virtual void release_savepoint( const std::string & name ) = 0;
   virtual void rollback_to_savepoint( const std::string & name ) = 0;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
