//-----------------------------------------------------------------------------

#ifndef DS_DB_IMPL_H
#define DS_DB_IMPL_H

//-----------------------------------------------------------------------------

#include <db/transactional.h>
#include <db/statement.h>

#include <map>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class impl : public transactional
{
public:

   virtual ~impl( void ) {}

   virtual const char * type( void ) const = 0;

   virtual void create( const std::string & name ) = 0;
   virtual void use( const std::string & name ) = 0;
   virtual bool drop( const std::string & name ) = 0;

   virtual statement operator()( const std::string & query,
                                 const name_list_t & parameters = {} ) = 0;

   virtual void execute_batch( const std::string & query ) = 0;

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
