//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_SQLITE_RESULT_H
#define DS_SQLITE_RESULT_H

//-----------------------------------------------------------------------------

#include <db/result.h>
#include <sqlite/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace sqlite
{

//-----------------------------------------------------------------------------

class rowset : public db::rowset::impl
{
   std::shared_ptr< stmt_t > m_stmt;
   int                       m_count;
   bool                      m_valid;

   int check_column( int index, int type_mask );

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

   virtual bool step( void ) override;

   virtual bool eof( void ) const override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
