//-----------------------------------------------------------------------------

#include <dsutil/env.h>
#include <cstring>
#include <windows.h>

//-----------------------------------------------------------------------------

namespace ds::util::env
{

//-----------------------------------------------------------------------------

std::string get( const char * name )
{
   char     * buffer = nullptr;
   int32_t    size   = GetEnvironmentVariableA( name, buffer, 0 );

   buffer = new char[ size ];

   GetEnvironmentVariableA( name, buffer, size );

   std::string message( buffer, size );

   delete[] buffer;

   return message;
}

//-----------------------------------------------------------------------------

dir_list::dir_list( const std::string & dirlist ) :
m_dirlist( dirlist )
{
}

//-----------------------------------------------------------------------------

dir_list::iterator::iterator( const char * dirlist )
{
   if ( dirlist )
   {
      m_dirlist = dirlist;
      m_token   = strtok_s( const_cast< char * >( m_dirlist.c_str() ), ";", &m_save );
   }
}

//-----------------------------------------------------------------------------

void dir_list::iterator::next_dir( void )
{
   m_token = strtok_s( nullptr, ";", &m_save );
}

//-----------------------------------------------------------------------------

const char * dir_list::iterator::operator*( void ) const
{
   return m_token;
}

//-----------------------------------------------------------------------------

dir_list::iterator dir_list::iterator::operator++( void )
{
   next_dir();
   return *this;
}

//-----------------------------------------------------------------------------

dir_list::iterator dir_list::iterator::operator++( int )
{
   auto it = *this;
   next_dir();
   return *this;
}

//-----------------------------------------------------------------------------

bool dir_list::iterator::operator==( const iterator & it ) const
{
   return **this == *it;
}

//-----------------------------------------------------------------------------

bool dir_list::iterator::operator!=( const iterator & it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

dir_list::iterator dir_list::begin( void ) const
{
   return { m_dirlist.c_str() };
}

//-----------------------------------------------------------------------------

dir_list::iterator dir_list::end( void ) const
{
   return {};
}

//-----------------------------------------------------------------------------

}
