//-----------------------------------------------------------------------------

#ifndef DS_MYSQL_ROW_H
#define DS_MYSQL_ROW_H

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

class row : public db::row::impl
{
   MYSQL       & m_mysql;
   MYSQL_RES   * m_mysql_res;
   MYSQL_FIELD * m_mysql_field;
   MYSQL_ROW     m_mysql_row;
   int           m_count;

   void check_column( int index, int type );

public:

   row( MYSQL & mysql );
   virtual ~row( void );

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
