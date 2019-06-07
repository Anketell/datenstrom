//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_POSITIONAL_STATEMENT_H
#define DS_MYSQL_POSITIONAL_STATEMENT_H

//-----------------------------------------------------------------------------

#include <mysql/statement_base.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class positional_statement : public statement_base
{
   MYSQL_BIND * m_mysql_bind = nullptr;
   int          m_bind_count = 0;

   void prepare_parameter_binding( void );

   void cleanup_parameters( void );
   int  check_parameter( int index );
   void set_parameter( int index, const char * s, size_t length );
   void internal_execute( void );

public:

   positional_statement( MYSQL & mysql, const std::string & sql );
   ~positional_statement( void );

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
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
