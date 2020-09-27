//-----------------------------------------------------------------------------

#include <utils/filesys_find.h>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------

namespace util
{

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
m_hfile( INVALID_HANDLE_VALUE ),
m_pattern_list( pattern_list ),
m_path( path )
{
   init_dir();
   next_file();
}

//-----------------------------------------------------------------------------

std::string find::iterator::full_path( void ) const
{
   std::string path = m_path;

   for ( auto & dir : m_dir_stack )
   {
      if ( dir.hfind == INVALID_HANDLE_VALUE )
         break;

      path += "/";
      path += dir.data.cFileName;
   }

   return path;
}

//-----------------------------------------------------------------------------

std::string find::iterator::operator*( void ) const
{
   return full_path() + "/" + m_file_data.cFileName;
}

//-----------------------------------------------------------------------------

bool find::iterator::next_child_dir( dir_t & dir )
{
   do
   {
      if ( *dir.data.cFileName != '.' && dir.data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
         return true;
   } while ( FindNextFileA( dir.hfind, &dir.data ) );

   FindClose( dir.hfind );
   dir.hfind = INVALID_HANDLE_VALUE;

   return false;
}

//-----------------------------------------------------------------------------

void find::iterator::init_dir( void )
{
   m_dir_stack.push_back( { INVALID_HANDLE_VALUE, full_path(), {} } );
   m_pattern_it = m_pattern_list.begin();
}

//-----------------------------------------------------------------------------

void find::iterator::next_dir( void )
{
   while ( !m_dir_stack.empty() )
   {
      dir_t & dir = m_dir_stack.back();

      if ( dir.hfind == INVALID_HANDLE_VALUE )
      {
         dir.hfind = FindFirstFileExA( ( dir.path + "/*" ).c_str(),
                                       FindExInfoBasic,
                                       &dir.data,
                                       FindExSearchLimitToDirectories,
                                       nullptr,
                                       0) ;

         if ( dir.hfind != INVALID_HANDLE_VALUE )
         {
            if ( next_child_dir( dir ) )
            {
               init_dir();
               break;
            }
         }
      }
      else
      {
         if ( FindNextFileA( dir.hfind, &dir.data ) )
         {
            if ( next_child_dir( dir ) )
            {
               init_dir();
               break;
            }
         }
         else
         {
            FindClose( dir.hfind );
            dir.hfind = INVALID_HANDLE_VALUE;
         }
      }
      if ( dir.hfind == INVALID_HANDLE_VALUE )
         m_dir_stack.pop_back();
   }
}

//-----------------------------------------------------------------------------

void find::iterator::next_file( void )
{
   for ( ;; )
   {
      if ( m_hfile == INVALID_HANDLE_VALUE )
      {
         if ( m_pattern_it == m_pattern_list.end() )
            break;

         m_hfile = FindFirstFileA( (f ull_path() + "/" + *m_pattern_it).c_str(), &m_file_data );

         if ( m_hfile != INVALID_HANDLE_VALUE )
            break;
      }
      else
      {
         if ( FindNextFileA( m_hfile, &m_file_data ) )
            break;

         FindClose( m_hfile );
         m_hfile = INVALID_HANDLE_VALUE;
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
   if ( m_hfile == INVALID_HANDLE_VALUE )
      return it.m_hfile == INVALID_HANDLE_VALUE;

   return **this == *it;
}

//-----------------------------------------------------------------------------

bool find::iterator::operator != ( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
