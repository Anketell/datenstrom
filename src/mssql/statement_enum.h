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

namespace ds::mssql
{

//-----------------------------------------------------------------------------

class statement_enum
{
   const std::string m_statements;

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

      struct token_t
      {
         const char * from;
         uint32_t     len;
      };

      static token_t next_token( const char * from );
      static token_t skip_to_end_token( const char * from );

      const std::string & m_statements;
      statement_t         m_statement;

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
template<> class iterator_traits< ds::mssql::statement_enum::iterator >
{
public:

   typedef int32_t                             difference_type;
   typedef ds::mssql::statement_enum::iterator value_type;
   typedef value_type *                        pointer;
   typedef value_type &                        reference;
   typedef forward_iterator_tag                iterator_category;
};
}

//-----------------------------------------------------------------------------
