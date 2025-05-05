//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <postgresql/types.h>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

class statement_base : public db::statement::impl
{
   int                     m_param_count;
   std::vector< uint32_t > m_param_types;
   std::vector< char * >   m_param_values;
   std::vector< uint64_t > m_param_data;
   std::vector< int >      m_param_lengths;
   std::vector< int >      m_param_formats;

   std::string m_name;

   void check_parameter( int index );

protected:

   std::shared_ptr< stmt_t > m_stmt;

   statement_base( PGconn * conn );
   virtual ~statement_base( void );

   void prepare( const std::string & sql );

   virtual void internal_execute( void );

   template< typename BI > void set_big_int( int index, BI bi );

public:

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
