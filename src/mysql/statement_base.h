//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <mysql.h>
#include <mysql/types.h>

//-----------------------------------------------------------------------------

namespace ds::mysql
{

//-----------------------------------------------------------------------------

class statement_base : public db::statement::impl
{
   MYSQL_BIND  * m_mysql_bind = nullptr;
   bind_info_t * m_bind_info  = nullptr;
   int           m_bind_count = 0;

   void prepare_parameter_binding( void );

   void check_parameter( int index );
   void set_parameter( int index, const char * s, size_t length );

protected:

   enum state_t { Preparing, Executed };

   std::shared_ptr< stmt_t > m_stmt;
   state_t                   m_state;

   statement_base( MYSQL & mysql );
   virtual ~statement_base( void );

   void prepare( const std::string & sql );

   virtual void internal_execute( void );

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

   virtual void set_parameter_null( int index ) override;

   virtual int parameter_count( void ) override;

   virtual void reset( void ) override;
   virtual void execute( void ) override;
   virtual db::rowset result( void ) override;
};

//-----------------------------------------------------------------------------

}
