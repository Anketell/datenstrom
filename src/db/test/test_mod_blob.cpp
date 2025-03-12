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

constexpr char blob_data[] = "The quick brown fox jumps over the lazy dog";

//-----------------------------------------------------------------------------

TEST_P( BLOB, should_create_and_read_blob )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create_blob" ) );

   {
      ds::db::statement insert = test_db( "test.insert_blob" );
      EXPECT_NO_THROW( insert << std::string( blob_data ) << ds::endr );

      std::string value;

      ds::db::statement query = test_db( "test.select_blob" );
      EXPECT_NO_THROW( query.result() >> value );

      EXPECT_EQ( value, blob_data );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

