//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/module.h>
#include <dlfcn.h>

//-----------------------------------------------------------------------------

namespace ds::util
{

//-----------------------------------------------------------------------------

constexpr char module::PATTERN[];

//-----------------------------------------------------------------------------

module::module( const std::string & path )
{
   m_handle = dlopen( path.c_str(), RTLD_LAZY | RTLD_LOCAL );
}

//-----------------------------------------------------------------------------

void * module::find_symbol( const std::string & name ) const
{
   if ( !m_handle )
      return nullptr;

   return dlsym( m_handle, name.c_str() );
}

//-----------------------------------------------------------------------------

void module::close( void )
{
   if ( m_handle )
   {
      dlclose( m_handle );
      m_handle = nullptr;
   }
}

//-----------------------------------------------------------------------------

}
