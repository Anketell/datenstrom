//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/context.h>
#include <db/test/test_mod_stdserial.h>
#include <db/test/test_mod_config.h>
#include <test_model/object.h>
#include <test_model/object_serialise.h>
#include <std/vector_serialise.h>
#include <std/tuple_serialise.h>
#include <test_utils/gtest.h>
#include <cmath>

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_insert_vector )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   std::vector< test::Object > data_vec;
   
   data_vec.insert( data_vec.end(), std::begin( data ), std::end( data ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_vec );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_read_vector )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   std::vector< test::Object > data_vec;
   
   data_vec.insert( data_vec.end(), std::begin( data ), std::end( data ) );

   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_vec );
   }

   {
      ds::db::rowset result = test_db( "test.results" ).result();

      std::vector< test::Object > db_data_vec;

      EXPECT_NO_THROW( result >> db_data_vec );

      EXPECT_EQ( db_data_vec, data_vec );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_insert_tuple )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   auto data_tuple = std::make_tuple( data[ 0 ].m_i8,
                                      data[ 0 ].m_i16,
                                      data[ 0 ].m_i32,
                                      data[ 0 ].m_i64,
                                      data[ 0 ].m_u8,
                                      data[ 0 ].m_u16,
                                      data[ 0 ].m_u32,
                                      data[ 0 ].m_u64,
                                      data[ 0 ].m_f,
                                      data[ 0 ].m_d,
                                      data[ 0 ].m_hello,
                                      data[ 0 ].m_date,
                                      data[ 0 ].m_time,
                                      data[ 0 ].m_datetime );

   
   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_tuple << ds::endr );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_read_tuple )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   auto data_tuple = std::make_tuple( data[ 0 ].m_i8,
                                      data[ 0 ].m_i16,
                                      data[ 0 ].m_i32,
                                      data[ 0 ].m_i64,
                                      data[ 0 ].m_u8,
                                      data[ 0 ].m_u16,
                                      data[ 0 ].m_u32,
                                      data[ 0 ].m_u64,
                                      data[ 0 ].m_f,
                                      data[ 0 ].m_d,
                                      data[ 0 ].m_hello,
                                      data[ 0 ].m_date,
                                      data[ 0 ].m_time,
                                      data[ 0 ].m_datetime );

   
   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_tuple << ds::endr );
   }

   {
      ds::db::rowset result = test_db( "test.results" ).result();

      std::tuple< int8_t, 
                  int16_t, 
                  int32_t, 
                  int64_t, 
                  uint8_t,
                  uint16_t,
                  uint32_t,
                  uint64_t,
                  double,
                  double,
                  std::string,
                  std::string,
                  std::string,
                  std::string > db_data_tuple;

      EXPECT_NO_THROW( result >> db_data_tuple >> ds::endr );

      EXPECT_EQ( std::get<  0 >( db_data_tuple ), std::get<  0 >( data_tuple ) );
      EXPECT_EQ( std::get<  1 >( db_data_tuple ), std::get<  1 >( data_tuple ) );
      EXPECT_EQ( std::get<  2 >( db_data_tuple ), std::get<  2 >( data_tuple ) );
      EXPECT_EQ( std::get<  3 >( db_data_tuple ), std::get<  3 >( data_tuple ) );
      EXPECT_EQ( std::get<  4 >( db_data_tuple ), std::get<  4 >( data_tuple ) );
      EXPECT_EQ( std::get<  5 >( db_data_tuple ), std::get<  5 >( data_tuple ) );
      EXPECT_EQ( std::get<  6 >( db_data_tuple ), std::get<  6 >( data_tuple ) );
      EXPECT_EQ( std::get<  7 >( db_data_tuple ), std::get<  7 >( data_tuple ) );
      EXPECT_LT( fabs( std::get<  8 >( db_data_tuple ) - std::get<  8 >( data_tuple ) ), 0.001 );
      EXPECT_EQ( std::get<  9 >( db_data_tuple ), std::get<  9 >( data_tuple ) );
      EXPECT_EQ( std::get< 10 >( db_data_tuple ), std::get< 10 >( data_tuple ) );
      EXPECT_EQ( std::get< 11 >( db_data_tuple ), std::get< 11 >( data_tuple ) );
      EXPECT_EQ( std::get< 12 >( db_data_tuple ), std::get< 12 >( data_tuple ) );
      EXPECT_EQ( std::get< 13 >( db_data_tuple ), std::get< 13 >( data_tuple ) );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_insert_optional )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   test::Object_opt data_opt = 
   {
      data[ 0 ].m_i8,
      std::nullopt,
      data[ 0 ].m_i32,
      std::nullopt,
      data[ 0 ].m_u8,
      std::nullopt,
      data[ 0 ].m_u32,
      std::nullopt,
      data[ 0 ].m_f,
      std::nullopt,
      data[ 0 ].m_hello,
      std::nullopt,
      data[ 0 ].m_time,
      std::nullopt
   };
   
   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_opt << ds::endr );
   }

   {
      test::Object_opt db_data_opt;

      ds::db::rowset result = test_db( "test.results" ).result();

      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_i8 = static_cast< int8_t >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_i32 = static_cast< int32_t >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_u8 = static_cast< uint8_t >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_u32 = static_cast< uint32_t >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_f = static_cast< double >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_hello = static_cast< std::string >( result );
      EXPECT_TRUE( result.get_null() );
      EXPECT_FALSE( result.get_null() );
      db_data_opt.m_time = static_cast< std::string >( result );
      EXPECT_TRUE( result.get_null() );
      result >> ds::endr;

      EXPECT_EQ( db_data_opt, data_opt );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------

TEST_P( StdSerial, should_read_optional )
{
   ds::db::context test_db( config->constr );

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
   EXPECT_NO_THROW( test_db.create( test_db_name ) );
   EXPECT_NO_THROW( test_db.use( test_db_name ) );

   EXPECT_NO_THROW( test_db.execute_batch( "test.create" ) );

   test::Object_opt data_opt = 
   {
      data[ 0 ].m_i8,
      std::nullopt,
      data[ 0 ].m_i32,
      std::nullopt,
      data[ 0 ].m_u8,
      std::nullopt,
      data[ 0 ].m_u32,
      std::nullopt,
      data[ 0 ].m_f,
      std::nullopt,
      data[ 0 ].m_hello,
      std::nullopt,
      data[ 0 ].m_time,
      std::nullopt
   };
   
   {
      ds::db::statement insert_test = test_db( "test.insert" );

      EXPECT_NO_THROW( insert_test << data_opt << ds::endr );
   }

   {
      test::Object_opt db_data_opt;

      ds::db::rowset result = test_db( "test.results" ).result();

      EXPECT_NO_THROW( result >> db_data_opt >> ds::endr );

      EXPECT_EQ( db_data_opt, data_opt );
   }

   EXPECT_NO_THROW( test_db.drop( test_db_name ) );
}

//-----------------------------------------------------------------------------
