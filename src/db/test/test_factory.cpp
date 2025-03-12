//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <test_utils/gtest.h>
#include <db/factory.h>
#include <db/connection.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace derived_db_1
{
class connection : public ds::db::impl
{
   std::string m_param;

public:

   static constexpr char TYPE[] = "derived_db_1";

   connection( const std::string & path ) : m_param( path ) {}

   virtual const char * type( void ) const override { return TYPE; }

   std::string param( void ) const { return m_param; }

   virtual void create( const std::string & name ) override {}
   virtual void use( const std::string & name ) override {}
   virtual bool drop( const std::string & name ) override { return true; }

   virtual ds::db::statement operator()( const std::string         & query,
                                         const ds::db::name_list_t & parameters = {} ) override
   {
      return std::shared_ptr< ds::db::statement::impl >();
   }

   virtual void execute_batch( const std::string & sql ) override {}

   virtual void begin_transaction( void ) override {}
   virtual void commit_transaction( void ) override {}
   virtual void rollback_transaction( void ) override {}

   virtual void savepoint( const std::string & name ) override {}
   virtual void release_savepoint( const std::string & name ) override {}
   virtual void rollback_to_savepoint( const std::string & name ) override {}
};

constexpr char connection::TYPE[];

}

//-----------------------------------------------------------------------------

namespace derived_db_2
{

class connection : public ds::db::impl
{
   std::string m_param;

public:

   static constexpr char TYPE[] = "derived_db_2";

   connection( const std::string & path ) : m_param( path ) {}

   virtual const char * type( void ) const override { return TYPE; }

   std::string param( void ) const { return m_param; }

   virtual void create( const std::string & name ) override {}
   virtual void use( const std::string & name ) override {}
   virtual bool drop( const std::string & name ) override { return true; }

   virtual ds::db::statement operator()( const std::string         & query,
                                         const ds::db::name_list_t & parameters = {} ) override
   {
      return std::shared_ptr< ds::db::statement::impl >();
   }

   virtual void execute_batch( const std::string & sql ) override {}

   virtual void begin_transaction( void ) override {}
   virtual void commit_transaction( void ) override {}
   virtual void rollback_transaction( void ) override {}

   virtual void savepoint( const std::string & name ) override {}
   virtual void release_savepoint( const std::string & name ) override {}
   virtual void rollback_to_savepoint( const std::string & name ) override {}
};

constexpr char connection::TYPE[];

}

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

template<> impl * factory_helper< derived_db_1::connection >::construct( const connect_params_t & params )
{
   auto location = params[ "location" ];
   return new derived_db_1::connection( location );
}

template<> impl * factory_helper< derived_db_2::connection >::construct( const connect_params_t & params )
{
   auto location = params[ "location" ];
   return new derived_db_2::connection( location );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, factory, should_register_and_create )
{
   ds::db::factory factory;

   factory.register_impl< derived_db_1::connection >();
   factory.register_impl< derived_db_2::connection >();

   ds::db::impl * db;

   {
      ds::connect_params_t params =
      {
         { "type",     "derived_db_1" },
         { "location", "parameters" }
      };

      EXPECT_NO_THROW( db = factory( params ) );
      EXPECT_EQ( typeid( *db ), typeid( derived_db_1::connection ) );

      derived_db_1::connection * ddb1;

      EXPECT_NE( ddb1 = dynamic_cast< derived_db_1::connection * >( db ), nullptr );
      EXPECT_STREQ( ddb1->param().c_str(), "parameters" );
   }

   EXPECT_NO_THROW( delete db );

   {
      ds::connect_params_t params =
      {
         { "type",     "derived_db_2" },
         { "location", "other_parameters" }
      };

      EXPECT_NO_THROW( db = factory( params ) );
      EXPECT_EQ( typeid( *db ), typeid( derived_db_2::connection ) );

      derived_db_2::connection * ddb2;

      EXPECT_NE( ddb2 = dynamic_cast< derived_db_2::connection * >( db ), nullptr );
      EXPECT_STREQ( ddb2->param().c_str(), "other_parameters" );
   }

   EXPECT_NO_THROW( delete db );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, factory, should_deregister )
{
   ds::db::factory factory;

   factory.register_impl< derived_db_1::connection >();
   factory.register_impl< derived_db_2::connection >();

   ds::db::impl * db;

   {
      ds::connect_params_t params =
      {
         { "type",     "derived_db_1" },
         { "location", "parameters" }
      };

      EXPECT_NO_THROW( db = factory( params ) );
      EXPECT_EQ( typeid( *db ), typeid( derived_db_1::connection ) );

      derived_db_1::connection * ddb1;

      EXPECT_NE( ddb1 = dynamic_cast< derived_db_1::connection * >( db ), nullptr );
      EXPECT_STREQ( ddb1->param().c_str(), "parameters" );
   }

   EXPECT_NO_THROW( delete db );

   factory.deregister_all();

   EXPECT_THROW( factory( { { "type", "derived_db_1" }, { "location", "parameters" } } ),
                 ds::db::factory::Not_found_exception );

   EXPECT_THROW( factory( { { "type", "derived_db_2" }, { "location", "other_parameters" } } ),
                 ds::db::factory::Not_found_exception );
}

//-----------------------------------------------------------------------------

NAMESPACE_TEST( db, factory, should_fail_unknown )
{
   ds::db::factory factory;

   EXPECT_THROW( factory( { { "type", "derived_db_1" }, { "location", "parameters" } } ),
                 ds::db::factory::Not_found_exception );

   EXPECT_THROW( factory( { { "type", "derived_db_2" }, { "location", "other_parameters" } } ),
                 ds::db::factory::Not_found_exception );
}

//-----------------------------------------------------------------------------
