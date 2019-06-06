//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_PREPARED_ROW_H
#define DS_MYSQL_PREPARED_ROW_H

//-----------------------------------------------------------------------------

#include <db/row.h>
#include <mysql/types.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace mysql
{

//-----------------------------------------------------------------------------

class prepared_row : public db::row::impl
{
   std::shared_ptr< stmt_t > m_stmt;
   bool                      m_good;

   void get_column( int              index,
                    enum_field_types type,
                    void           * p,
                    size_t           length,
                    int              is_unsigned = 0 );

public:

   prepared_row( std::shared_ptr< stmt_t > stmt );

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
