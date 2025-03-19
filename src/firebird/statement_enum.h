//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <string>
#include <cstdint>

//-----------------------------------------------------------------------------

namespace ds::firebird
{

//-----------------------------------------------------------------------------

class statement_enum
{
   const std::string & m_statements;

public:

   statement_enum( const std::string & statements );

   class iterator
   {
   public:

      struct statement_t
      {
         const char * from = nullptr;
         uint32_t     len  = 0;
      };

   private:

      const std::string & m_statements;
      statement_t         m_statement;
      std::string         m_separator = ";";

      bool is_term_statement( void );
      void next_statement( void );

   public:

      iterator( const std::string & statements = "" );

      const statement_t & operator*( void ) const;
      const statement_t * operator->( void ) const;

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
template<> class iterator_traits< ds::firebird::statement_enum::iterator >
{
public:

   typedef int32_t                                difference_type;
   typedef ds::firebird::statement_enum::iterator value_type;
   typedef value_type *                           pointer;
   typedef value_type &                           reference;
   typedef forward_iterator_tag                   iterator_category;
};
}

//-----------------------------------------------------------------------------
