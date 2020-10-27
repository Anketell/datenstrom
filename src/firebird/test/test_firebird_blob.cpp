//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <db/transaction.h>
#include <firebird/test/firebird_test_data.h>

//-----------------------------------------------------------------------------

constexpr char blob_create[] =
R"(

CREATE TABLE Object (
   data BLOB )

)";

constexpr char blob_insert[] = "INSERT INTO Object VALUES( ? )";

constexpr char blob_data[] = "The quick brown fox jumps over the lazy dog";

//-----------------------------------------------------------------------------

TEST( firebird_blob, should_create_and_read_blob )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( blob_create ) );

   {
      ds::db::transaction txn( test_db );

      ds::db::statement insert = test_db( blob_insert );
      EXPECT_NO_THROW( insert << blob_data << ds::endr );

      std::string value;

      ds::db::statement query = test_db( "SELECT * FROM Object" );
      EXPECT_NO_THROW( query.result() >> value );

      EXPECT_EQ( value, blob_data );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

