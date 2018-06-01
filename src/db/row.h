//-----------------------------------------------------------------------------

#ifndef DS_DB_ROW_H
#define DS_DB_ROW_H

//-----------------------------------------------------------------------------

#include <ds/ds.h>
#include <memory>
#include <vector>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

typedef std::vector< std::string > name_list_t;

//-----------------------------------------------------------------------------

class row : public ds::istream
{
public:

   class impl
   {
   public:

      virtual void get_column( int index, int8_t & );
      virtual void get_column( int index, int16_t & );
      virtual void get_column( int index, int32_t & );
      virtual void get_column( int index, int64_t & );

      virtual void get_column( int index, uint8_t & );
      virtual void get_column( int index, uint16_t & );
      virtual void get_column( int index, uint32_t & );
      virtual void get_column( int index, uint64_t & );

      virtual void get_column( int index, double & );
      virtual void get_column( int index, std::string & );

      virtual bool step( void ) = 0;
      virtual operator bool ( void ) const = 0;
   };

private:

   std::shared_ptr< impl > m_impl;
   int                     m_column;

public:

   row( void );
   row( std::shared_ptr< impl > impl );

   virtual ds::istream & operator >> ( int8_t & ) override;
   virtual ds::istream & operator >> ( int16_t & ) override;
   virtual ds::istream & operator >> ( int32_t & ) override;
   virtual ds::istream & operator >> ( int64_t & ) override;

   virtual ds::istream & operator >> ( uint8_t & ) override;
   virtual ds::istream & operator >> ( uint16_t & ) override;
   virtual ds::istream & operator >> ( uint32_t & ) override;
   virtual ds::istream & operator >> ( uint64_t & ) override;

   virtual ds::istream & operator >> ( double & ) override;
   virtual ds::istream & operator >> ( std::string & ) override;

   virtual void endr( void ) override;

   bool step( void );

   bool operator == ( const row & r ) const;
   bool operator != ( const row & r ) const;

   operator bool ( void ) const;
   operator bool ( void );

   template< typename T > operator T ( void )
   {
      T t;
      *this >> t;
      return t;
   }

};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
