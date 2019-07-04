//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_NAMED_STATEMENT_H
#define DS_MYSQL_NAMED_STATEMENT_H

//-----------------------------------------------------------------------------

#include <mysql/statement_base.h>
#include <sstream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class named_statement : public statement_base
{
   MYSQL               & m_mysql;
   const db::name_list_t m_names;
   std::stringstream     m_values;

   const char * check_parameter( int index );
   void set_parameter( int index, const char * s, size_t length );
   void internal_execute( void );

public:

   named_statement( MYSQL                 & mysql,
                    const std::string     & sql,
                    const db::name_list_t & parameters );

   ~named_statement( void );

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
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
