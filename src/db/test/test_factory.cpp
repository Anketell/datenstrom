//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/factory.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

class derived_db_1 : public ds::db::impl
{
   std::string m_param;

public:

   static constexpr char TYPE[] = "derived_db_1";

   derived_db_1( std::string param ) : m_param( param ) {}

   std::string param( void ) const { return m_param; }

   virtual const char * type( void ) const override
   {
      return TYPE;
   }

   virtual void create( const std::string & name ) override {}
   virtual void use( const std::string & name ) override {}
   virtual bool drop( const std::string & name ) override { return true; }

   virtual ds::db::statement operator()( const std::string         & query,
                                         const ds::db::name_list_t & parameters = {} ) override
   {
      return std::shared_ptr< ds::db::statement::impl >();
   }

   virtual void begin_transaction( void ) override {}
   virtual void commit_transaction( void ) override {}
   virtual void rollback_transaction( void ) override {}

   virtual void savepoint( const std::string & name ) override {}
   virtual void release_savepoint( const std::string & name ) override {}
   virtual void rollback_to_savepoint( const std::string & name ) override {}
};

constexpr char derived_db_1::TYPE[];

//-----------------------------------------------------------------------------

class derived_db_2 : public ds::db::impl
{
   std::string m_param;

public:

   static constexpr char TYPE[] = "derived_db_2";

   derived_db_2( std::string param ) : m_param( param ) {}

   std::string param( void ) const { return m_param; }

   virtual const char * type( void ) const override
   {
      return TYPE;
   }

   virtual void create( const std::string & name ) override {}
   virtual void use( const std::string & name ) override {}
   virtual bool drop( const std::string & name ) override { return true; }

   virtual ds::db::statement operator()( const std::string         & query,
                                         const ds::db::name_list_t & parameters = {} ) override
   {
      return std::shared_ptr< ds::db::statement::impl >();
   }

   virtual void begin_transaction( void ) override {}
   virtual void commit_transaction( void ) override {}
   virtual void rollback_transaction( void ) override {}

   virtual void savepoint( const std::string & name ) override {}
   virtual void release_savepoint( const std::string & name ) override {}
   virtual void rollback_to_savepoint( const std::string & name ) override {}

};

constexpr char derived_db_2::TYPE[];

//-----------------------------------------------------------------------------

TEST( db_factory, should_register_and_create )
{
   ds::db::factory factory;

   factory.register_db< derived_db_1 >();
   factory.register_db< derived_db_2 >();

   ds::db::impl * db;

   EXPECT_NO_THROW( db = factory( "derived_db_1:parameters" ) );

   derived_db_1 * ddb1;

   EXPECT_NE( ddb1 = dynamic_cast< derived_db_1 * >( db ), nullptr );
   EXPECT_STREQ( ddb1->param().c_str(), "parameters" );

   EXPECT_NO_THROW( delete db );

   EXPECT_NO_THROW( db = factory( "derived_db_2:other_parameters" ) );

   derived_db_2 * ddb2;

   EXPECT_NE( ddb2 = dynamic_cast< derived_db_2 * >( db ), nullptr );
   EXPECT_STREQ( ddb2->param().c_str(), "other_parameters" );

   EXPECT_NO_THROW( delete db );
}

//-----------------------------------------------------------------------------

TEST( db_factory, should_fail_unknown )
{
   ds::db::factory factory;

   EXPECT_THROW( factory( "derived_db_1:parameters" ), ds::db::factory::Not_found_exception );
   EXPECT_THROW( factory( "derived_db_2:other_parameters" ), ds::db::factory::Not_found_exception );
}

//-----------------------------------------------------------------------------

TEST( ds_factory, should_fail_bad_connection_string )
{
   ds::db::factory factory;

   EXPECT_THROW( factory( "derived_db_1" ), std::runtime_error );
}


//-----------------------------------------------------------------------------
