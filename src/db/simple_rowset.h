//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <db/rowset.h>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

class simple_rowset : public rowset::impl
{
   uint64_t m_value;
   bool     m_valid;

   void check_column( int index );
   template< typename I > I get_integer( int index );

public:

   simple_rowset( uint64_t value );
   ~simple_rowset( void );

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
