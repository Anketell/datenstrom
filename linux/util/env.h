//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <iterator>
#include <memory>

//-----------------------------------------------------------------------------

namespace util
{

//-----------------------------------------------------------------------------

namespace env
{

//-----------------------------------------------------------------------------

const char * get( const char * name );

//-----------------------------------------------------------------------------

class dir_list
{
   const char * m_dirlist;

public:

   dir_list( const char * dirlist );

   class iterator
   {
      std::string   m_dirlist;
      const char  * m_token = nullptr;
      char        * m_save;

      void next_dir( void );

   public:

      iterator( const char * dirlist = nullptr );

      const char * operator*( void ) const;

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
template<> class iterator_traits< util::env::dir_list::iterator >
{
public:

   typedef int32_t                       difference_type;
   typedef util::env::dir_list::iterator value_type;
   typedef value_type *                  pointer;
   typedef value_type &                  reference;
   typedef forward_iterator_tag          iterator_category;
};
}

//-----------------------------------------------------------------------------
