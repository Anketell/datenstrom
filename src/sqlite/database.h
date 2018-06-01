//-----------------------------------------------------------------------------

#ifndef DS_SQLITE_DATABASE_H
#define DS_SQLITE_DATABASE_H

//-----------------------------------------------------------------------------

#include <db/database.h>
#include <sqlite3.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

class database : public db::database
{
   sqlite3 * m_db;

public:

   database( const std::string & filename );

   virtual ~database( void );

   virtual db::statement operator()( const std::string     & query,
                                     const db::name_list_t & parameters = {} ) override;

   virtual void begin_transaction( void ) override;
   virtual void commit_transaction( void ) override;
   virtual void rollback_transaction( void ) override;

   virtual void savepoint( const std::string & ) override;
   virtual void release_savepoint( const std::string & ) override;
   virtual void rollback_to_savepoint( const std::string & ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
