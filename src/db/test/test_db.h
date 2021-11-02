//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef TEST_DB_H
#define TEST_DB_H

//-----------------------------------------------------------------------------

#include <db/impl.h>

//-----------------------------------------------------------------------------

struct Test_result : public ds::db::result::impl
{
   int m_column_count;
   int m_step_calls;
   int m_row_count;

   Test_result( int column_count = 0, int row_count = 0 )
   {
      m_column_count = column_count;
      m_row_count    = row_count;
      m_step_calls   = 0;
   }

   int column_count( void ) const
   {
      return m_column_count;
   }

   int rows_affected( void ) const
   {
      return m_row_count;
   }

   void get_column( int index, int8_t & i )
   {
   }

   void get_column( int index, int16_t & i )
   {
   }

   void get_column( int index, int32_t & i )
   {
   }

   void get_column( int index, int64_t & i )
   {
   }

   void get_column( int index, uint8_t & u )
   {
   }

   void get_column( int index, uint16_t & u )
   {
   }

   void get_column( int index, uint32_t & u )
   {
   }

   void get_column( int index, uint64_t & u )
   {
   }

   void get_column( int index, double & d )
   {
   }

   void get_column( int index, std::string & s )
   {
   }

   virtual bool step( void ) override
   {
       ++m_step_calls;
       return !this->eof();
   }

   virtual bool eof( void ) const
   {
       return m_step_calls >= m_row_count;
   }
};

//-----------------------------------------------------------------------------

struct Test_statement : public ds::db::statement::impl
{
   int m_column_count;
   int m_row_count;
   int m_reset_calls;
   int m_execute_calls;
   int m_result_calls;

   Test_statement( int column_count = 1, int row_count = 0 )
   {
      m_column_count  = column_count;
      m_row_count     = row_count;
      m_reset_calls   = 0;
      m_execute_calls = 0;
      m_result_calls  = 0;
   }

   void set_parameter( int index, int8_t )
   {
   }

   void set_parameter( int index, int16_t )
   {
   }

   void set_parameter( int index, int32_t )
   {
   }

   void set_parameter( int index, int64_t )
   {
   }

   void set_parameter( int index, uint8_t )
   {
   }

   void set_parameter( int index, uint16_t )
   {
   }

   void set_parameter( int index, uint32_t )
   {
   }

   void set_parameter( int index, uint64_t )
   {
   }

   void set_parameter( int index, double )
   {
   }

   void set_parameter( int index, const char * )
   {
   }

   void set_parameter( int index, const std::string & )
   {
   }

   virtual int parameter_count( void ) override
   {
       return 0;
   }

   virtual void reset( void ) override
   {
      m_reset_calls++;
   }

   virtual uint64_t execute( void ) override
   {
      m_execute_calls++;
      return 0;
   }

   virtual ds::db::result result( void ) override
   {
      m_result_calls++;
      return ds::db::result( std::make_shared< Test_result >( m_column_count, m_row_count ) );
   }
};

//-----------------------------------------------------------------------------

struct Test_db : public ds::db::impl
{
   static constexpr char TYPE[] = "Test_db";

   int m_begin_txn_calls;
   int m_commit_txn_calls;
   int m_rollback_txn_calls;
   int m_savepoint_calls;
   int m_release_savepoint_calls;
   int m_rollback_to_savepoint_calls;

   Test_db( void )
   {
      m_begin_txn_calls    = 0;
      m_commit_txn_calls   = 0;
      m_rollback_txn_calls = 0;

      m_savepoint_calls             = 0;
      m_release_savepoint_calls     = 0;
      m_rollback_to_savepoint_calls = 0;
   }

   virtual const char * type( void ) const override
   {
      return TYPE;
   }

   virtual void create( const std::string & name ) override
   {
   }

   virtual void use( const std::string & name ) override
   {
   }

   virtual bool drop( const std::string & name ) override
   {
      return true;
   }

   virtual ds::db::statement operator()( const std::string & query, const ds::db::name_list_t & parameters )
   {
       return ds::db::statement( std::make_shared< Test_statement >( atoi( query.c_str() ) ) );
   }

   virtual void execute_batch( const std::string & sql ) override {}

   virtual void begin_transaction( void ) override
   {
      m_begin_txn_calls++;
   }

   virtual void commit_transaction( void ) override
   {
      m_commit_txn_calls++;
   }

   virtual void rollback_transaction( void ) override
   {
      m_rollback_txn_calls++;
   }

   void savepoint( const std::string & ) override
   {
      m_savepoint_calls++;
   }

   void release_savepoint( const std::string & ) override
   {
      m_release_savepoint_calls++;
   }

   void rollback_to_savepoint( const std::string & ) override
   {
      m_rollback_to_savepoint_calls++;
   }
};

//-----------------------------------------------------------------------------

#endif
