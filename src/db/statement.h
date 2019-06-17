//-----------------------------------------------------------------------------

#ifndef DS_DB_STATEMENT_H
#define DS_DB_STATEMENT_H

//-----------------------------------------------------------------------------

#include <db/row.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

typedef std::vector< std::string > name_list_t;

//-----------------------------------------------------------------------------

class statement : public ds::ostream
{
public:

   class impl
   {
   protected:

      virtual ~impl( void ) {}

   public:

      virtual void set_parameter( int index, int8_t ) = 0;
      virtual void set_parameter( int index, int16_t ) = 0;
      virtual void set_parameter( int index, int32_t ) = 0;
      virtual void set_parameter( int index, int64_t ) = 0;

      virtual void set_parameter( int index, uint8_t ) = 0;
      virtual void set_parameter( int index, uint16_t ) = 0;
      virtual void set_parameter( int index, uint32_t ) = 0;
      virtual void set_parameter( int index, uint64_t ) = 0;

      virtual void set_parameter( int index, double ) = 0;

      virtual void set_parameter( int index, const char * ) = 0;
      virtual void set_parameter( int index, const std::string & ) = 0;

      virtual int parameter_count( void ) = 0;

      virtual void reset( void ) = 0;
      virtual uint32_t execute( void ) = 0;
      virtual row result( void ) = 0;
   };

private:

   std::shared_ptr< impl > m_impl;
   int                     m_parameter;

public:

   class iterator
   {
   protected:

      row m_row;

   public:

      iterator( void );
      iterator( row row );

      row operator*( void );

      iterator & operator++( void );
      iterator & operator++( int );

      bool operator==( const iterator & it ) const;
      bool operator!=( const iterator & it ) const;
   };

   statement( std::shared_ptr< impl > impl );

   virtual ds::ostream & operator << ( int8_t ) override;
   virtual ds::ostream & operator << ( int16_t ) override;
   virtual ds::ostream & operator << ( int32_t ) override;
   virtual ds::ostream & operator << ( int64_t ) override;

   virtual ds::ostream & operator << ( uint8_t ) override;
   virtual ds::ostream & operator << ( uint16_t ) override;
   virtual ds::ostream & operator << ( uint32_t ) override;
   virtual ds::ostream & operator << ( uint64_t ) override;

   virtual ds::ostream & operator << ( double ) override;

   virtual ds::ostream & operator << ( const char * ) override;
   virtual ds::ostream & operator << ( const std::string & ) override;

   virtual void endr( void ) override;

   void reset( void );
   uint32_t execute( void );
   row result( void );

   iterator begin( void );
   iterator end( void );

   template< typename T > statement operator()( T & t )
   {
      *this << t;
      return *this;
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
