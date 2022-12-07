//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/filesys.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds::filesys
{

//-----------------------------------------------------------------------------

bool exists( const char * path )
{
   WIN32_FIND_DATA FindFileData;

   HANDLE handle = FindFirstFile( path, &FindFileData );

   if( handle == INVALID_HANDLE_VALUE )
      return false;

   FindClose( handle );

   return true;
}

//-----------------------------------------------------------------------------

static std::string error_message( int32_t error )
{
   std::string message;

   char * buffer;

   FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM     |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  error,
                  MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                  reinterpret_cast< LPTSTR >( & buffer ),
                  0,
                  nullptr );

   message = buffer;

   LocalFree( buffer );

   return message;
}

//-----------------------------------------------------------------------------

void remove( const char * path )
{
   if ( !DeleteFileA( path ) )
   {
      int32_t error = GetLastError();
      if ( error != ERROR_FILE_NOT_FOUND )
         throw std::runtime_error( error_message( error ) );
   }
}

//-----------------------------------------------------------------------------

std::string temp_directory( void )
{
   char buffer[ MAX_PATH + 1 ];
   GetTempPath( sizeof( buffer ), buffer );
   return buffer;
}

//-----------------------------------------------------------------------------

find::find( const std::string & path, const pattern_list_t & pattern_list ) :
m_path( path ),
m_pattern_list( pattern_list )
{
}

//-----------------------------------------------------------------------------

find::iterator find::begin( void ) const
{
   return { m_path, m_pattern_list };
}

//-----------------------------------------------------------------------------

find::iterator find::end( void ) const
{
   return {};
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

         m_hfile = FindFirstFileA( ( full_path() + "/" + *m_pattern_it ).c_str(), &m_file_data );

         if ( m_hfile != INVALID_HANDLE_VALUE )
         {
            if ( ( m_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
               break;
            else
               continue;
         }
      }
      else
      {
         if ( FindNextFileA( m_hfile, &m_file_data ) )
         {
            if ( ( m_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
               break;
            else
               continue;
         }
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
