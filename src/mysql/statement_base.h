//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_STATEMENT_BASE_H
#define DS_MYSQL_STATEMENT_BASE_H

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <mysql/mysql.h>
#include <mysql/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class statement_base : public db::statement::impl
{
protected:

   enum state_t { Preparing, Executed };

   std::shared_ptr< stmt_t > m_stmt;
   state_t                   m_state;

   statement_base( MYSQL & mysql, const std::string & sql );

   virtual void internal_execute( void );

public:

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
