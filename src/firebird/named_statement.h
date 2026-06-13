//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <firebird/statement_base.h>
#include <map>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

class named_statement : public statement_base
{
   const db::name_list_t     m_names;
   std::multimap< int, int > m_param_map;

   std::string get_pos_sql( const std::string     & sql,
                            const db::name_list_t & parameters );

   void check_parameter( int index );

   template< typename T > void internal_set_parameter( int index, T t );

public:

   named_statement( transactional         & transactional,
                    const std::string     & sql,
                    const db::name_list_t & parameters );

   virtual ~named_statement( void );

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
};

//-----------------------------------------------------------------------------

}
