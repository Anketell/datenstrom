//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_DB_CONNECTION_H
#define DS_DB_CONNECTION_H

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <db/factory.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class connection : public impl
{
   static factory m_factory;
   static bool    m_initialized;

   static void init( void );

   std::shared_ptr< impl > m_impl;

public:

   static void enroll_db_path_list( const std::string & path_list );

   connection( const std::string & connect_string );
   connection( const connect_params_t & connect_params );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name ) override;
   virtual void use( const std::string & name ) override;
   virtual bool drop( const std::string & name ) override;

   virtual statement operator()( const std::string & query,
                                 const name_list_t & parameters = {} ) override;

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
