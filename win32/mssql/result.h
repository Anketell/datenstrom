//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_RESULT_H
#define DS_MSSQL_RESULT_H

//-----------------------------------------------------------------------------

#include <db/result.h>
#include <mssql/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

class result : public db::result::impl
{
   std::shared_ptr< stmt_t > m_stmt;
   bool                      m_valid;

   void check_column( int index );

   template< typename T > void get_column( int index, int c_type, T & t );

public:

   result( std::shared_ptr< stmt_t > stmt );

   virtual int column_count( void ) const override;
   virtual int rows_affected( void ) const override;

   virtual void get_column( int index, int8_t & ) override;
   virtual void get_column( int index, int16_t & ) override;
   virtual void get_column( int index, int32_t & ) override;
   virtual void get_column( int index, int64_t & ) override;

   virtual void get_column( int index, uint8_t & ) override;
   virtual void get_column( int index, uint16_t & ) override;
   virtual void get_column( int index, uint32_t & ) override;
   virtual void get_column( int index, uint64_t & ) override;

   virtual void get_column( int index, double & ) override;
   virtual void get_column( int index, std::string & ) override;

   virtual bool step( void ) override;

   virtual operator bool ( void ) const override;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
