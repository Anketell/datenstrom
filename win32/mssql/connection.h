//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_CONNECTION_H
#define DS_MSSQL_CONNECTION_H

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <windows.h>
#include <tchar.h>
#include <sal.h>
#include <sql.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
   static std::string create_connection_string( const std::string & server, 
                                                int                 port   );

   SQLHENV  m_henv = SQL_NULL_HENV;
   SQLHDBC  m_hdbc = SQL_NULL_HDBC;
   SQLHSTMT m_stmt = SQL_NULL_HSTMT;

   int m_transactions = 0;

   std::string m_database;

   void init(const std::string& connection_string);
   void cleanup( void );

public:

   static constexpr char TYPE[] = "mssql";

   connection( const std::string & server, int port );

   virtual ~connection( void );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name ) override;
   virtual void use( const std::string & name = "master" ) override;
   virtual bool drop( const std::string & name ) override;

   virtual db::statement operator()( const std::string     & query,
                                     const db::name_list_t & parameters = {} ) override;

   virtual void execute_batch( const std::string & query ) override;

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
