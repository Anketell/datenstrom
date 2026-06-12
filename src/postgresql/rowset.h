//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/rowset.h>
#include <postgresql/types.h>
#include <vector>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

class rowset : public db::rowset::impl
{
   std::shared_ptr< stmt_t > m_stmt;
   std::vector< int32_t >    m_column_types;
   int                       m_column_count = 0;
   int                       m_row_count;
   int                       m_cursor;
   bool                      m_valid;

   void configure_buffer( void );

   void check_column( int index );

   int64_t get_int( int index );

public:

   rowset( std::shared_ptr< stmt_t > stmt );
   virtual ~rowset( void );

   virtual int column_count( void ) const override;
   virtual int rows_affected( void ) const override;

   virtual void get_column( int index, int8_t & ) override;
   virtual void get_column( int index, int16_t & ) override;
   virtual void get_column( int index, int32_t & ) override;
   virtual void get_column( int index, int64_t & ) override;

   virtual void get_column( int index, uint8_t & ) override;
   virtual void get_column( int index, uint16_t & ) override;
   virtual void get_column( int index, uint32_t & ) override;
   virtual void get_column( int index, uint64_t & ) override;

   virtual void get_column( int index, double & ) override;
   virtual void get_column( int index, std::string & ) override;

   virtual bool get_column_null( int index ) override;

   virtual bool step( void ) override;

   virtual bool eof( void ) const override;
};

//-----------------------------------------------------------------------------

}
