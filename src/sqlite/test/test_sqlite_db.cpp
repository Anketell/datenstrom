//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <sqlite/database.h>
#include <sqlite/test/test_data.h>
#include <test_model/object_serialise.h>

//-----------------------------------------------------------------------------

TEST( sqlite_db, should_open_good_path )
{
   EXPECT_NO_THROW( ds::sqlite::database test_db( tmp_file ) );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db, should_fail_bad_path )
{
   EXPECT_THROW( ds::sqlite::database test_db( bad_file ), std::runtime_error );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_execute_simple )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   ds::db::statement create_test = test_db( create );
   EXPECT_NO_THROW( create_test.execute() );

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_fail_bad_sql )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   EXPECT_THROW( test_db( bad_sql ), std::runtime_error );

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_execute_parameters )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      EXPECT_NO_THROW( insert_test << data[ 0 ] );
      EXPECT_NO_THROW( insert_test.execute() );

      EXPECT_NO_THROW( insert_test << 1 << 1 << 1 << 1 << 1
                                   << 1 << 1 << 1 << 1 << 1 << "hello2" );
      EXPECT_NO_THROW( insert_test.execute() );
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_fail_too_many_parameters )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      insert_test << data[ 0 ];

      EXPECT_THROW( insert_test << ( int8_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int16_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int32_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( int64_t )1, std::runtime_error );

      EXPECT_THROW( insert_test << ( uint8_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint16_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint32_t )1, std::runtime_error );
      EXPECT_THROW( insert_test << ( uint64_t )1, std::runtime_error );

      EXPECT_THROW( insert_test << 8.5, std::runtime_error );

      EXPECT_THROW( insert_test << data[ 0 ].m_hello, std::runtime_error );
      EXPECT_THROW( insert_test << "hello", std::runtime_error );
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_fail_not_enough_parameters )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      EXPECT_THROW( insert_test.execute(), std::runtime_error );
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_provide_result_row )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::row row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         if ( row )
         {
            row.step();
         }
      }
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_statement, should_fail_bad_reset )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test1 = test_db( create );
      ds::db::statement create_test2 = test_db( create );

      EXPECT_NO_THROW( create_test1.execute() );

      EXPECT_THROW( create_test2.execute(), std::runtime_error );
      EXPECT_THROW( create_test2.reset(), std::runtime_error );
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_row, should_provide_data )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::row row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         if ( row )
         {
            EXPECT_NO_THROW( row >> o_db );
         }

         EXPECT_EQ( o, o_db );

         if ( row )
         {
            row.step();
         }
      }
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_row, should_fail_not_available )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   ds::db::statement create_test = test_db( create );

   EXPECT_EQ( ds::db::row(), create_test.result() );

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_row, should_fail_wrong_column_count )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::row row;

      EXPECT_NO_THROW( row = results_test.result() );

      for ( auto o : data )
      {
         Object o_db = {};

         if ( row )
         {
            EXPECT_NO_THROW( row >> o_db );
            EXPECT_THROW( row >> o_db, std::runtime_error );
         }

         EXPECT_EQ( o, o_db );

         if ( row )
         {
            row.step();
         }
      }
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------

TEST( sqlite_db_row, should_fail_wrong_column_type )
{
   unlink( tmp_file );

   ds::sqlite::database test_db( tmp_file );

   {
      ds::db::statement create_test = test_db( create );
      EXPECT_NO_THROW( create_test.execute() );
   }

   {
      ds::db::statement insert_test = test_db( insert );

      for ( auto o : data )
      {
         EXPECT_NO_THROW( insert_test << o );
         EXPECT_NO_THROW( insert_test.execute() );
      }
   }

   {
      ds::db::statement results_test = test_db( results );

      ds::db::row row;

      EXPECT_NO_THROW( row = results_test.result() );

      std::string hello;

      EXPECT_THROW( row >> hello, std::runtime_error );
   }

   unlink( tmp_file );
}

//-----------------------------------------------------------------------------
