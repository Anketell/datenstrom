//-----------------------------------------------------------------------------
//
// Copyright (C) 2026 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <optional>
#include <ds/ds.h>

//-----------------------------------------------------------------------------

namespace std
{

//-----------------------------------------------------------------------------

template< typename T >
ds::ostream & operator << ( ds::ostream & out, const optional< T > & o )
{
   if ( o.has_value() )
      out << *o;
   else
      out.put_null();

   return out;
}

//-----------------------------------------------------------------------------

template< typename T >
ds::istream & operator >> ( ds::istream & in, optional< T > & o )
{
   if ( in.get_null() )
      o.reset();
   else
      o = in;

   return in;
}

//-----------------------------------------------------------------------------

}