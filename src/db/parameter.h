//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef UTIL_PARAMETER_H
#define UTIL_PARAMETER_H

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds::util::parameter
{

//-----------------------------------------------------------------------------

class enumerator
{
   const std::string & m_sql;
   const std::string   m_delimiters;

public:

   enumerator( const std::string & sql, const char * delimiters = ":@$" );

   class iterator
   {
   public:

      struct parameter_t
      {
         uint32_t from = 0;
         uint32_t len  = 0;
      };

   private:

      const std::string & m_sql;
      const std::string & m_delimiters;
      parameter_t         m_parameter;

      void next_parameter( void );

   public:

      iterator( const std::string & sql = "", const std::string & delimiters = "");

      const parameter_t & operator*( void ) const;
      const parameter_t * operator->( void ) const;

      iterator operator++( void );
      iterator operator++( int );

      bool operator==( const iterator & it ) const;
      bool operator!=( const iterator & it ) const;
   };

   iterator begin( void ) const;
   iterator end( void ) const;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

namespace std
{
template<> class iterator_traits< ds::util::parameter::enumerator::iterator >
{
public:

   typedef int32_t                                   difference_type;
   typedef ds::util::parameter::enumerator::iterator value_type;
   typedef value_type *                              pointer;
   typedef value_type &                              reference;
   typedef forward_iterator_tag                      iterator_category;
};
}

//-----------------------------------------------------------------------------

#endif
