//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <ds/factory.h>

//-----------------------------------------------------------------------------

class base_t
{
   int m_id;

protected:

   base_t( int id ) : m_id( id ) {}

public:

   int id( void ) const { return m_id; }

   virtual std::string name( void ) const = 0;
};

//-----------------------------------------------------------------------------

class derived_1_t : public base_t
{
public:

   derived_1_t( int id ) : base_t( id ) {}

   virtual std::string name( void ) const { return "derived_1"; }
};

//-----------------------------------------------------------------------------

class derived_2_t : public base_t
{
public:

   derived_2_t( int id ) : base_t( id ) {}

   virtual std::string name( void ) const { return "derived_2"; }
};

//-----------------------------------------------------------------------------

typedef ds::factory< base_t, std::string, int > test_factory_t;

//-----------------------------------------------------------------------------

TEST( ds_factory, should_register_and_create )
{
   test_factory_t factory;

   factory.register_type< derived_1_t >( "derived_1" );
   factory.register_type< derived_2_t >( "derived_2" );

   base_t * base;

   EXPECT_NO_THROW( base = factory( "derived_1", 37 ) );
   EXPECT_STREQ( base->name().c_str(), "derived_1" );
   EXPECT_EQ( base->id(), 37 );
   EXPECT_NO_THROW( delete base );

   EXPECT_NO_THROW( base = factory( "derived_2", 45 ) );
   EXPECT_STREQ( base->name().c_str(), "derived_2" );
   EXPECT_EQ( base->id(), 45 );
   EXPECT_NO_THROW( delete base );
}

//-----------------------------------------------------------------------------

TEST( ds_factory, should_fail_unknown_type )
{
   test_factory_t factory;

   EXPECT_THROW( factory( "derived_1", 37 ), test_factory_t::Not_found_exception );
   EXPECT_THROW( factory( "derived_2", 45 ), test_factory_t::Not_found_exception );
}


//-----------------------------------------------------------------------------
