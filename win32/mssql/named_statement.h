//-----------------------------------------------------------------------------

#ifndef DS_MSSQL_NAMED_STATEMENT_H
#define DS_MSSQL_NAMED_STATEMENT_H

//-----------------------------------------------------------------------------

#include <mssql/statement_base.h>
#include <map>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mssql
{

//-----------------------------------------------------------------------------

class named_statement : public statement_base
{
   const db::name_list_t     m_names;
   std::multimap< int, int > m_param_map;

   std::string get_pos_sql( const std::string     & sql,
                            const db::name_list_t & parameters );

   void check_parameter( int index );

   template< typename T > void internal_set_parameter( int index, T t );

public:

   named_statement( SQLHDBC hdbc,
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
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
