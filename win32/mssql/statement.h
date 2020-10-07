//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_STATEMENT_H
#define DS_MSSQL_STATEMENT_H

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

class statement : public db::statement::impl
{
   enum state_t { Preparing, Executed };

   std::shared_ptr< stmt_t > m_stmt;
   std::vector< int >        m_parameters;
   int                       m_count;
   state_t                   m_state;

   int check_parameter( int index );

public:

   statement( const std::string     & sql,
              const db::name_list_t & parameters );

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
   virtual uint64_t execute( void ) override;
   virtual db::result result( void ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
