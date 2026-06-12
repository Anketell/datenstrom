//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <test_utils/gtest.h>
#include <db/context.h>
#include <db/test/test_mod_blob.h>
#include <db/test/test_mod_config.h>

//-----------------------------------------------------------------------------

constexpr char data[] = "\0\1\2\3\4\5\6\7\0\1\2\3\4\5\6\7\0\1\2\3\4\5\6\7";

//-----------------------------------------------------------------------------

TEST_P( BLOB, should_write_and_read )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create_blob" ) );

   std::string blob_data( data, sizeof( data ) );

   {
      ds::db::statement insert = test_db( "test.insert_blob" );
      EXPECT_NO_THROW( insert << blob_data << ds::endr );

      std::string value;

      ds::db::statement query = test_db( "test.select_blob" );
      EXPECT_NO_THROW( query.result() >> value );

      EXPECT_EQ( value, blob_data );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( BLOB, should_throw_null_value_when_invalid )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create_blob" ) );

   std::string blob_data( data, sizeof( data ) );

   {
      EXPECT_NO_THROW( test_db.execute_batch( "test.insert_null_blob" ) );

      std::string value;

      ds::db::statement query = test_db( "test.select_blob" );
      EXPECT_THROW( query.result() >> value, ds::null_value );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( BLOB, should_get_null_when_invalid )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create_blob" ) );

   std::string blob_data( data, sizeof( data ) );

   {
      EXPECT_NO_THROW( test_db.execute_batch( "test.insert_null_blob" ) );

      std::string value;

      ds::db::statement query = test_db( "test.select_blob" );
      EXPECT_TRUE( query.result().get_null() );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( BLOB, should_not_get_null_when_valid )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create_blob" ) );

   std::string blob_data( data, sizeof( data ) );

   {
      ds::db::statement insert = test_db( "test.insert_blob" );
      EXPECT_NO_THROW( insert << blob_data << ds::endr );

      std::string value;

      ds::db::statement query = test_db( "test.select_blob" );
      auto result = query.result();
      EXPECT_FALSE( result.get_null() );
      EXPECT_NO_THROW( result >> value );

      EXPECT_EQ( value, blob_data );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

