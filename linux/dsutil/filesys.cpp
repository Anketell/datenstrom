//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/filesys.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <sys/stat.h>

//-----------------------------------------------------------------------------

namespace ds::util::filesys
{

//-----------------------------------------------------------------------------

bool exists( const char * path )
{
   return access( path, F_OK ) == 0;
}

//-----------------------------------------------------------------------------

void remove( const char * path )
{
   if ( unlink( path ) != 0 )
   {
      if ( errno != ENOENT )
         throw std::runtime_error( strerror( errno ) );
   }
}

//-----------------------------------------------------------------------------

find::find( const std::string & path, const pattern_list_t & pattern_list ) :
m_path( path ),
m_pattern_list( pattern_list )
{
}

//-----------------------------------------------------------------------------

const std::string    find::iterator::null_path;
const pattern_list_t find::iterator::null_pattern;

//-----------------------------------------------------------------------------

find::iterator::iterator( const std::string & path, const pattern_list_t & pattern_list ) :
m_pattern_list( pattern_list ),
m_path( path )
{
   init_dir();
   next_file();
}

//-----------------------------------------------------------------------------

find::iterator::~iterator(void)
{
   for ( auto & dir : m_dir_stack )
   {
      if ( dir.index == INVALID )
         break;

      globfree( &dir.glob );
      dir.index = INVALID;
   }

   globfree( &m_glob );
   m_index = INVALID;
}

//-----------------------------------------------------------------------------

std::string find::iterator::full_path( void ) const
{
   const char * path = m_path.c_str();

   for ( auto & dir : m_dir_stack )
   {
      if ( dir.index == INVALID )
         break;

      path = dir.glob.gl_pathv[ dir.index ];
   }

   return path;
}

//-----------------------------------------------------------------------------

std::string find::iterator::operator*( void ) const
{
   if ( m_index == INVALID )
      return "";

   return m_glob.gl_pathv[ m_index ];
}

//-----------------------------------------------------------------------------

bool find::iterator::next_child_dir( dir_t & dir )
{
   struct stat st;

   do
   {
      if (!lstat( dir.glob.gl_pathv[ dir.index ], &st) && !S_ISLNK( st.st_mode ) )
         return true;
   } while ( ++dir.index < dir.glob.gl_pathc );

   globfree( &dir.glob );
   dir.index = INVALID;

   return false;
}

//-----------------------------------------------------------------------------

void find::iterator::init_dir( void )
{
   m_dir_stack.push_back( { {}, INVALID, full_path() } );
   m_pattern_it = m_pattern_list.begin();
}

//-----------------------------------------------------------------------------

void find::iterator::next_dir( void )
{
   while ( !m_dir_stack.empty() )
   {
      dir_t & dir = m_dir_stack.back();

      if ( dir.index == INVALID )
      {
         if ( !glob((dir.path + "/*" ).c_str(), GLOB_ONLYDIR, nullptr, &dir.glob ) )
         {
            dir.index = 0;
            if ( next_child_dir( dir ) )
            {
               init_dir();
               break;
            }
         }
      }
      else
      {
         if ( ++dir.index < dir.glob.gl_pathc )
         {
            if ( next_child_dir( dir ) )
            {
               init_dir();
               break;
            }
         }
         else
         {
            globfree( &dir.glob );
            dir.index = INVALID;
         }
      }
      if ( dir.index == INVALID )
         m_dir_stack.pop_back();
   }
}

//-----------------------------------------------------------------------------

void find::iterator::next_file( void )
{
   for ( ;; )
   {
      if ( m_index == INVALID )
      {
         if ( m_pattern_it == m_pattern_list.end() )
            break;

         if ( !glob( ( full_path() + "/" + *m_pattern_it ).c_str(), 0, nullptr, &m_glob ) )
         {
            m_index = 0;
            break;
         }
      }
      else
      {
         if ( ++m_index < m_glob.gl_pathc )
            break;

         globfree( &m_glob );
         m_index = INVALID;
      }
      if ( ++m_pattern_it == m_pattern_list.end() )
         next_dir();
   }
}

//-----------------------------------------------------------------------------

find::iterator & find::iterator::operator++( void )
{
   next_file();
   return *this;
}

//-----------------------------------------------------------------------------

bool find::iterator::operator == ( const iterator & it ) const
{
   if (m_index == INVALID)
      return it.m_index == INVALID;

   return **this == *it;
}

//-----------------------------------------------------------------------------

bool find::iterator::operator != ( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}
