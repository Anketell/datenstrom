//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <glob.h>

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
   static constexpr int INVALID = -1;

   const std::string    m_path;
   const pattern_list_t m_pattern_list;

public:

   class iterator
   {
      struct dir_t
      {
         glob_t      glob;
         int         index;
         std::string path;
      };

      typedef std::deque< dir_t > dir_stack_t;

      static const std::string    null_path;
      static const pattern_list_t null_pattern;

      const pattern_list_t & m_pattern_list;
      const std::string    & m_path;

      dir_stack_t m_dir_stack;

      glob_t m_glob  = {};
      int    m_index = INVALID;

      pattern_list_t::const_iterator m_pattern_it;

      bool next_child_dir( dir_t & dir );

      std::string full_path( void ) const;

      void init_dir (void );
      void next_dir( void) ;
      void next_file (void );

   public:

      iterator( const std::string    & path         = null_path,
                const pattern_list_t & pattern_list = null_pattern );

      ~iterator( void );

      std::string operator*( void ) const;

      iterator & operator++( void );

      bool operator == ( const iterator & it ) const;
      bool operator != ( const iterator & it ) const;
   };

   find( const std::string & path, const pattern_list_t & pattern_list );

   iterator begin( void ) const { return { m_path, m_pattern_list }; }
   iterator end( void )   const { return {}; }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

namespace std
{
template<> class iterator_traits< util::filesys::find::iterator >
{
public:

   typedef int32_t              difference_type;
   typedef std::string          value_type;
   typedef value_type *         pointer;
   typedef value_type &         reference;
   typedef forward_iterator_tag iterator_category;
};
}

//-----------------------------------------------------------------------------
