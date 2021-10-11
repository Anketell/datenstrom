//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <vector>
#include <deque>
#include <cstdint>
#include <string>
#include <windows.h>

//-----------------------------------------------------------------------------

namespace util
{

//-----------------------------------------------------------------------------

namespace filesys
{

//-----------------------------------------------------------------------------

bool exists( const char * path );
void remove( const char * path );

//-----------------------------------------------------------------------------

typedef std::vector< std::string > pattern_list_t;

//-----------------------------------------------------------------------------

class find
{
   const std::string    m_path;
   const pattern_list_t m_pattern_list;

public:

   class iterator
   {
      struct dir_t
      {
         HANDLE           hfind;
         std::string      path;
         WIN32_FIND_DATAA data;
      };

      typedef std::deque< dir_t > dir_stack_t;

      static const std::string    null_path;
      static const pattern_list_t null_pattern;

      const pattern_list_t & m_pattern_list;
      const std::string    & m_path;

      dir_stack_t m_dir_stack;

      HANDLE           m_hfile;
      WIN32_FIND_DATAA m_file_data;

      pattern_list_t::const_iterator m_pattern_it;

      bool next_child_dir( dir_t & dir) ;

      std::string full_path( void ) const;

      void init_dir( void );
      void next_dir( void );
      void next_file( void );

   public:

      iterator( const std::string    & path         = null_path,
                const pattern_list_t & pattern_list = null_pattern );

      std::string operator*( void ) const;

      iterator & operator++( void );

      bool operator == ( const iterator & it ) const;
      bool operator != ( const iterator & it ) const;
   };

   find( const std::string & path, const pattern_list_t & pattern_list );

   iterator begin( void ) const;
   iterator end( void )   const;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

template<> class std::iterator_traits< util::filesys::find::iterator >
{
public:

   typedef int32_t              difference_type;
   typedef std::string          value_type;
   typedef value_type *         pointer;
   typedef value_type &         reference;
   typedef forward_iterator_tag iterator_category;
};

//-----------------------------------------------------------------------------
