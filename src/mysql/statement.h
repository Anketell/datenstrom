//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_STATEMENT_H
#define DS_MYSQL_STATEMENT_H

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <mysql/mysql.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class statement : public db::statement::impl
{
   enum state_t { Preparing, Executed };

   MYSQL               & m_mysql;
   const std::string     m_query;
   const db::name_list_t m_names;
   std::stringstream     m_values;
   state_t               m_state;

   const char * check_parameter( int index );
   void set_parameter( int index, const char * s, size_t length );
   void internal_execute( void );

public:

   statement( MYSQL                 & mysql,
              const std::string     & sql,
              const db::name_list_t & parameters );

   ~statement( void );

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
