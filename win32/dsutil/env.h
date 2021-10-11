//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <iterator>
#include <string>

//-----------------------------------------------------------------------------

namespace ds::util::env
{

//-----------------------------------------------------------------------------

std::string get( const char * name );

//-----------------------------------------------------------------------------

class dir_list
{
   const std::string m_dirlist;

public:

   dir_list( const std::string & dirlist );

   class iterator
   {
      std::string   m_dirlist;
      const char  * m_token = nullptr;
      char        * m_save  = nullptr;

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

template<> class std::iterator_traits< ds::util::env::dir_list::iterator >
{
public:

   typedef int32_t                           difference_type;
   typedef ds::util::env::dir_list::iterator value_type;
   typedef value_type *                      pointer;
   typedef value_type &                      reference;
   typedef forward_iterator_tag              iterator_category;
};

//-----------------------------------------------------------------------------
