//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <windows.h>
#include <tchar.h>
#include <sal.h>
#include <sql.h>

#include <functional>

//-----------------------------------------------------------------------------

namespace ds::mssql
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
   typedef std::function< void( void ) > guarded_fn;

   static std::string create_connection_string( const std::string & server,
                                                const std::string & instance,
                                                int                 port );

   static std::string create_connection_string( const std::string & user_id,
                                                const std::string & password,
                                                const std::string & server,
                                                const std::string & instance,
                                                int                 port );

   SQLHENV  m_henv = SQL_NULL_HENV;
   SQLHDBC  m_hdbc = SQL_NULL_HDBC;
   SQLHSTMT m_stmt = SQL_NULL_HSTMT;

   int m_transactions = 0;

   std::string m_connection_string;
   std::string m_database;

   void init(const std::string& connection_string);
   void cleanup( void );

   void guard( guarded_fn fn );

public:

   static constexpr char TYPE[] = "mssql";

   connection( const std::string & server, const std::string & instance, int port );
   connection( const std::string & user_id,
               const std::string & password,
               const std::string & server, const std::string & instance, int port );

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
