//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_DB_RESULT_H
#define DS_DB_RESULT_H

//-----------------------------------------------------------------------------

#include <ds/ds.h>
#include <memory>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

class result : public ds::istream
{
public:

   class impl
   {
   protected:

      virtual ~impl( void ) {}

   public:

      virtual int column_count( void ) const = 0;
      virtual int rows_affected( void ) const = 0;

      virtual void get_column( int index, int8_t & ) = 0;
      virtual void get_column( int index, int16_t & ) = 0;
      virtual void get_column( int index, int32_t & ) = 0;
      virtual void get_column( int index, int64_t & ) = 0;

      virtual void get_column( int index, uint8_t & ) = 0;
      virtual void get_column( int index, uint16_t & ) = 0;
      virtual void get_column( int index, uint32_t & ) = 0;
      virtual void get_column( int index, uint64_t & ) = 0;

      virtual void get_column( int index, double & ) = 0;
      virtual void get_column( int index, std::string & ) = 0;

      virtual bool step( void ) = 0;
      virtual operator bool ( void ) const = 0;
   };

private:

   std::shared_ptr< impl > m_impl;
   int                     m_column;

public:

   result( void );
   result( std::shared_ptr< impl > impl );

   int column_count( void ) const;
   int rows_affected( void ) const;

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

   virtual operator bool ( void ) const override;
   operator bool ( void );
   
   bool operator==( const result &  ) const;

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
