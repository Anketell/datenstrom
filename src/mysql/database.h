//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_DATABASE_H
#define DS_MYSQL_DATABASE_H

//-----------------------------------------------------------------------------

#include <db/database.h>
#include <mysql/mysql.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class database : public db::database
{
  MYSQL m_mysql;

public:

   database( const std::string & name,
             const std::string & server,
             const std::string & username,
             const std::string & password,
             uint16_t            port = 3306 );

   database( const std::string & server,
             const std::string & username,
             const std::string & password,
             uint16_t            port = 3306 );

   virtual ~database( void );

   virtual void create( const std::string & name );
   virtual void use( const std::string & name );
   virtual bool drop( const std::string & name );

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
