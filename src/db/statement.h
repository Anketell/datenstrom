//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_DB_STATEMENT_H
#define DS_DB_STATEMENT_H

//-----------------------------------------------------------------------------

#include <db/result.h>
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
      virtual void execute( void ) = 0;
      virtual db::rowset result( void ) = 0;
   };

private:

   std::shared_ptr< impl > m_impl;
   int                     m_parameter;

public:

   class iterator
   {
   protected:

      rowset m_rowset;

   public:

      iterator( void );
      iterator( db::rowset rowset );

      db::rowset operator*( void );
      db::rowset * operator->( void );

      iterator & operator++( void );
      iterator & operator++( int );

      bool operator==( const iterator & it ) const;
      bool operator!=( const iterator & it ) const;
   };

   statement( std::shared_ptr< impl > impl );

   int parameter_count( void ) const;

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
   void execute( void );
   db::rowset result( void );

   iterator begin( void );
   iterator end( void );

   template< typename T > statement & operator()( const T & t )
   {
      *this << t;
      return *this;
   }

   template< typename T, typename ... Rest >
   statement & operator()( const T & t, const Rest & ... rest )
   {
      *this << t;
      return operator()( rest... );
   }

   template< typename T,
             std::enable_if_t< ds::has_get_from< T > || std::is_same_v< T, db::rowset >,
                               bool
                             > = false
           >
   operator T ( void )
   {
      return result();
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

namespace std
{
template<> class iterator_traits< ds::db::statement::iterator >
{
public:

   typedef int32_t              difference_type;
   typedef ds::db::rowset       value_type;
   typedef value_type *         pointer;
   typedef value_type &         reference;
   typedef forward_iterator_tag iterator_category;
};
}

//-----------------------------------------------------------------------------

#endif
