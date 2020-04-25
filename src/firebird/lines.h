//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_LINES_H
#define DS_FIREBIRD_LINES_H

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class lines
{
   const std::string & m_lines;

public:

   lines( const std::string & lines );

   class iterator
   {
   public:

      struct line_t
      {
         const char * from = nullptr;
         const char * to   = nullptr;
      };

   private:

      const std::string & m_lines;
      line_t              m_line;

      void next_line( void );

   public:

      iterator( const std::string & lines = "" );

      const line_t & operator*( void ) const;
      const line_t * operator->( void ) const;

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

template<> class std::iterator_traits< ds::firebird::lines::iterator >
{
public:

   typedef int32_t                      difference_type;
   typedef ds::firebird::lines::iterator value_type;
   typedef value_type *                 pointer;
   typedef value_type &                 reference;
   typedef forward_iterator_tag         iterator_category;
};


//-----------------------------------------------------------------------------

#endif
