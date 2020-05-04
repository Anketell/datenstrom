//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_STATEMENTS_H
#define DS_FIREBIRD_STATEMENTS_H

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class statements
{
   const std::string & m_statements;

public:

   statements( const std::string & statements );

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

      void next_line( void );

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

}

//-----------------------------------------------------------------------------

namespace std
{
template<> class iterator_traits< ds::firebird::statements::iterator >
{
public:

   typedef int32_t                            difference_type;
   typedef ds::firebird::statements::iterator value_type;
   typedef value_type *                       pointer;
   typedef value_type &                       reference;
   typedef forward_iterator_tag               iterator_category;
};
}

//-----------------------------------------------------------------------------

#endif
