//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/connection.h>
#include <db/sql_map.h>
#include <stdexcept>
#include <deque>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

class context : public connection
{
public:

   class unsupported_db_type : public std::runtime_error
   {
   public:

      unsupported_db_type( const std::string & type ) :
      type( type ),
      std::runtime_error( "Unsupported database type: " + type ) {}

      const std::string type;
   };

   class unknown_sql : public std::runtime_error
   {
   public:

      unknown_sql( const std::string & type, const std::string & key ) :
      type( type ),
      key( key ),
      std::runtime_error( "Unknown " + type + " sql: " + key ) {}

      const std::string type;
      const std::string key;
   };

private:

   static sql::map_t m_sql_map;
   static bool    m_initialized;

   static void init( void );

   std::deque< sql::lookup_t > m_sql_lookup;

   void common_constructor( void );

public:

   static void enroll_sql_path_list( const std::string & path_list );
   static void clear_sql_path_list( void );

   context( connection & con );
   context( const std::string & connect_string );
   context( const ds::connect_params_t & connect_params );

   virtual statement operator()( const std::string & query_key,
                                 const name_list_t & parameters = {} ) override;

   virtual void execute_batch( const std::string & query_key ) override;

   const char * lookup( const std::string & key );
};

//-----------------------------------------------------------------------------

}
