//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <postgresql/statement_base.h>

//-----------------------------------------------------------------------------

namespace ds::pg
{

//-----------------------------------------------------------------------------

class named_statement : public statement_base
{
   const db::name_list_t m_names;

   std::string get_pos_sql( const std::string     & sql,
                            const db::name_list_t & parameters );

public:

   named_statement( PGconn                * conn,
                    const std::string     & sql,
                    const db::name_list_t & parameters );

   virtual int parameter_count( void ) override;
};

//-----------------------------------------------------------------------------

}
