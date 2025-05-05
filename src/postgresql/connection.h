//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/impl.h>

//-----------------------------------------------------------------------------

struct pg_conn;
typedef struct pg_conn PGconn;

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
   PGconn * m_conn;

   std::string m_user_id;
   std::string m_password;
   std::string m_server;
   std::string m_port;

   bool m_in_transaction = false;

   void detach( void );
   void use( void );

public:

   static constexpr char TYPE[] = "postgresql";

   connection( const std::string & user_id,
               const std::string & password,
               const std::string & server, int port );

   virtual ~connection( void );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name ) override;
   virtual void use( const std::string & name ) override;
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
