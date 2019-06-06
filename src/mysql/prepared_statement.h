//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_PREPARED_STATEMENT_H
#define DS_MYSQL_PREPARED_STATEMENT_H

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <mysql/mysql.h>
#include <mysql/types.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class prepared_statement : public db::statement::impl
{
   enum state_t { Preparing, Executed };

   std::shared_ptr< stmt_t > m_stmt;
   MYSQL_BIND              * m_mysql_bind  = nullptr;
   int                       m_bind_count  = 0;
   state_t                   m_state;

   void prepare_parameter_binding( void );
   void prepare_result_fields( void );

   void cleanup_parameters( void );
   int  check_parameter( int index );
   void set_parameter( int index, const char * s, size_t length );
   void internal_execute( void );

public:

   prepared_statement( MYSQL & mysql, const std::string & sql );
   ~prepared_statement( void );

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

   virtual void     reset( void ) override;
   virtual uint32_t execute( void ) override;
   virtual db::row  result( void ) override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
