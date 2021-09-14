//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_RESULT_H
#define DS_FIREBIRD_RESULT_H

//-----------------------------------------------------------------------------

#include <db/result.h>
#include <db/transaction.h>
#include <firebird/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class transactional;

//-----------------------------------------------------------------------------

class result : public db::result::impl
{
   std::shared_ptr< stmt_t >              m_stmt;
   transactional                        & m_transactional;
   std::unique_ptr< ds::db::transaction > m_transaction;
   bool                                   m_valid;

   void read_blob( int index, std::string & s );
   void check_column( int index );

   template< typename BI > BI get_big_int( int index );
   template< typename I > I get_integer( int index );

public:

   result( std::shared_ptr< stmt_t >              stmt,
           transactional                        & trasnactional,
           std::unique_ptr< ds::db::transaction > transaction = nullptr );
   ~result( void );

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

   virtual bool step( void ) override;

   virtual operator bool ( void ) const override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
