//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_STATEMENT_H
#define DS_FIREBIRD_STATEMENT_H

//-----------------------------------------------------------------------------

#include <db/statement.h>
#include <firebird/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class transactional;

//-----------------------------------------------------------------------------

class positional_statement : public db::statement::impl
{
   enum state_t { Preparing, Executed };

   transactional           & m_transactional;
   std::shared_ptr< stmt_t > m_stmt;
   XSQLDA                  * m_xsqlda = nullptr;
   std::vector< int >        m_parameters;
   int                       m_count;
   state_t                   m_state;

   void check_parameter( int index );
   void internal_execute( void );

public:

   positional_statement( transactional     & transactional,
                         const std::string & sql );

   virtual ~positional_statement( void );

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
