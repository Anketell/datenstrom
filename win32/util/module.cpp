//-----------------------------------------------------------------------------

#include <util/module.h>
#include <windows.h>

//-----------------------------------------------------------------------------

namespace util
{

//-----------------------------------------------------------------------------

constexpr char module::PATTERN[];

//-----------------------------------------------------------------------------

module::module( const std::string & path )
{
   m_handle =LoadLibraryA( path.c_str() );
}

//-----------------------------------------------------------------------------

void * module::find_symbol( const std::string & name ) const
{
   if ( !m_handle )
      return nullptr;

   return GetProcAddress( reinterpret_cast< HMODULE >( m_handle ), name.c_str() );
}

//-----------------------------------------------------------------------------

void module::close( void )
{
   if ( m_handle )
   {
      FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
      m_handle = nullptr;
   }
}

//-----------------------------------------------------------------------------

}
