//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_PREPARED_RESULT_H
#define DS_MYSQL_PREPARED_RESULT_H

//-----------------------------------------------------------------------------

#include <db/result.h>
#include <mysql/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class result : public db::result::impl
{
   std::shared_ptr< stmt_t > m_stmt;
   int                       m_count;
   bool                      m_valid;

   void get_column( int              index,
                    enum_field_types type,
                    void           * p,
                    size_t           length,
                    int              is_unsigned = 0 );

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
