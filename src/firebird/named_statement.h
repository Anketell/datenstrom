//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <firebird/statement_base.h>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

class named_statement : public statement_base
{
   struct stmt_meta_t
   {
      int32_t * index;
      XSQLDA  * in;
      XSQLDA  * out;
      int32_t   type;
   };

   std::string get_pos_sql( const std::string     & sql,
                            const db::name_list_t & parameters,
                            int                   * index );

   void get_stmt_meta( const std::string     & sql,
                       const db::name_list_t & parameters,
                       stmt_meta_t           * meta );

   std::string wrap_sql( const std::string     & sql,
                         const db::name_list_t & parameters,
                         int32_t               * type  );

public:

   named_statement( transactional         & transactional,
                    const std::string     & sql,
                    const db::name_list_t & parameters );
};

//-----------------------------------------------------------------------------

}
