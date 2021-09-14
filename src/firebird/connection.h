//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_CONNECTION_H
#define DS_FIREBIRD_CONNECTION_H

//-----------------------------------------------------------------------------

#include <db/impl.h>
#include <firebird/transactional.h>
#
//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class connection : public db::impl
{
   firebird::transactional m_transactional;
   std::string             m_server;
   std::string             m_path;
   std::string             m_attached_db;
   std::string             m_ext;
   uint16_t                m_port;
   int                     m_dialect;
   std::string             m_dpb;
   size_t                  m_username_offset;
   size_t                  m_password_offset;

   void construct_dpb( const std::string & username,
                       const std::string & password,
                       int                 dialect );

   std::string get_local_path( const std::string & name ) const;
   std::string get_full_path( const std::string & name ) const;

   void detach( void );

   void set_sql_dialect( int dialect );

public:

   static constexpr char TYPE[] = "firebird";

   connection( const std::string & server,
               const std::string & path,
               const std::string & username,
               const std::string & password,
               const std::string & ext,
               uint16_t            port = 3050,
               int                 dialect = 3 );

   virtual ~connection( void );

   virtual const char * type( void ) const override;

   virtual void create( const std::string & name );
   virtual void use( const std::string & name );
   virtual bool drop( const std::string & name );

   virtual db::statement operator()( const std::string     & query,
                                     const db::name_list_t & parameters = {} ) override;

   virtual void execute_batch( const std::string & query ) override;

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
