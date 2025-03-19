//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <db/transaction.h>
#include <firebird/types.h>
#include <functional>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

typedef std::function< void( XSQLDA * ) > describe_fn_t;

//-----------------------------------------------------------------------------

struct transactional;

//-----------------------------------------------------------------------------

class statement_base : public db::statement::impl
{
   XSQLDA * m_xsqlda = nullptr;

   std::unique_ptr< ds::db::transaction > m_tmp_transaction;

   template< typename BI > void set_big_int( int index, BI bi );
   void set_string( int index, uint32_t len, const char * data );

   XSQLDA * prepare_xsqlda( describe_fn_t describe_fn );
   void     allocate_xsqlvars( XSQLDA * xsqlda );

   void prepare_statement_type( void );
   void prepare_parameter_buffer( void );
   void prepare_result_buffer( void );

   void check_parameter( int index );

   void execute_statement( void );
   void execute_procedure( void );
   void internal_execute( void );

   void begin_tmp_transaction( void );

   void write_blob( int index, uint32_t len, const char * data );

protected:

   transactional           & m_transactional;
   std::shared_ptr< stmt_t > m_stmt;

   XSQLDA * prepare_parameter_xsqlda( isc_stmt_handle stmt );
   XSQLDA * prepare_result_xqslda( isc_stmt_handle stmt );

   int get_statement_type( isc_stmt_handle stmt );

   void prepare( const std::string & sql );

   statement_base( transactional & transactional );

public:

   virtual ~statement_base( void );

   virtual void set_parameter( int index, int8_t ) override;
   virtual void set_parameter( int index, int16_t ) override;
   virtual void set_parameter( int index, int32_t ) override;
   virtual void set_parameter( int index, int64_t ) override;

   virtual void set_parameter( int index, uint8_t ) override;
   virtual void set_parameter( int index, uint16_t ) override;
   virtual void set_parameter( int index, uint32_t ) override;
   virtual void set_parameter( int index, uint64_t ) override;

   virtual void set_parameter( int index, double ) override;

   virtual void set_parameter( int index, const char * ) override;
   virtual void set_parameter( int index, const std::string & ) override;

   virtual int parameter_count( void ) override;

   virtual void reset( void ) override;
   virtual void execute( void ) override;
   virtual db::rowset result( void ) override;
};

//-----------------------------------------------------------------------------

}
