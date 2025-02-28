//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_STATEMENT_BASE_H
#define DS_MSSQL_STATEMENT_BASE_H

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <mssql/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

class statement_base : public db::statement::impl
{
   class buffer
   {
      size_t m_length = 0;
      void * m_data   = nullptr;

   public:

      template< typename T > T * data( void );
      void resize( size_t size );
      ~buffer( void );
   };

   std::vector< stmt_t::desc_t > m_parameters;
   std::vector< buffer >         m_buffers;

   buffer & check_parameter( int index );

   void bind_time( int index, time_t t );

   template< typename T > void bind_parameter( int index, int c_type, const T & t );

   void prepare_parameter_desc( void );

protected:

   std::shared_ptr< stmt_t > m_stmt;

   statement_base( SQLHDBC hdbc );

   void prepare( const std::string & sql );

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

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
