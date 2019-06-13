//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_CONNECTION_H
#define DS_MYSQL_CONNECTION_H

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <mysql/mysql.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
  MYSQL m_mysql;

public:

   static constexpr char TYPE[] = "mysql";

   connection( const std::string & name,
               const std::string & server,
               const std::string & username,
               const std::string & password,
               uint16_t            port = 3306 );

   connection( const std::string & server,
               const std::string & username,
               const std::string & password,
               uint16_t            port = 3306 );

   connection( db::connect_params_t params );

   virtual ~connection( void );

   virtual const char * type( void ) const override;

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
