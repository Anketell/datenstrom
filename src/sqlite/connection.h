//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <sqlite3.h>

//-----------------------------------------------------------------------------

namespace ds::sqlite
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
   const std::string m_path;
   sqlite3         * m_db        = nullptr;
   int               m_txn_count = 0;

   std::string get_full_path( const std::string & name ) const;

   void close( void );

public:

   static constexpr char TYPE[] = "sqlite";

   connection( const std::string & path );

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
