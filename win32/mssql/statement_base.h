//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_STATEMENT_BASE_H
#define DS_MSSQL_STATEMENT_BASE_H

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

class statement_base : public db::statement::impl
{
   enum state_t { Preparing, Executed };
   
   struct buffer_t
   {
      int    length;
      void * data;

      ~buffer_t( void )
      {
         if ( length )
         {
            free( data );
            length = 0;
         }
      }
   };

   std::shared_ptr< stmt_t >     m_stmt;
   std::vector< stmt_t::desc_t > m_parameters;
   std::vector< buffer_t >       m_buffers;
   state_t                       m_state;

   buffer_t & check_parameter( int index, int size );

   template< typename T > void bind_parameter( int index, int c_type, const T & t );

   void prepare_parameter_buffer( void );
   void prepare_result_buffer( void );

protected:

   statement_base( SQLHDBC hdbc );

   void prepare( const std::string & sql );

public:

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
