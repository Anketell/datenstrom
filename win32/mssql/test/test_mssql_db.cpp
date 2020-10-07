//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/connection.h>
#include <mssql/test/mssql_test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( mssql_db, should_return_type )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_STREQ( test_db.type(), "mssql" );
}

//-----------------------------------------------------------------------------

TEST( mssql_db, should_create_good_path )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mssql_db, should_execute_batch )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );

   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( batch ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mssql_db, should_fail_create_bad_con_str )
{
   EXPECT_THROW( ds::db::connection test_db( "mssql://" ), std::invalid_argument );
}

//-----------------------------------------------------------------------------

TEST( mssql_db_statement, should_execute_simple )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db( create ) );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST( mssql_db_statement, should_fail_create_bad_sql )
{
   ds::db::connection test_db( test_con_str );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_THROW( test_db( bad_sql ), std::runtime_error );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
