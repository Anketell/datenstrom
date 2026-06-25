//-----------------------------------------------------------------------------
//
// Copyright (C) 2026 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <vector>
#include <tuple>
#include <ds/ds.h>
#include <iostream>
#include <utility>

//-----------------------------------------------------------------------------

namespace std
{

//-----------------------------------------------------------------------------

#if __cplusplus > 202000

//-----------------------------------------------------------------------------

template< typename... T >
ds::istream & operator >> ( ds::istream & in, tuple< T... > & t )
{
   [ & ]< size_t... S >( index_sequence< S... > )
   {
      ( ( in >> get< S >( t ) ), ... );
   }( make_index_sequence< tuple_size_v< tuple< T... > > >{} );

   return in;
}

//-----------------------------------------------------------------------------

template< typename... T >
ds::ostream & operator << ( ds::ostream & out, const tuple< T... > & t )
{
   [ & ]< size_t... S >( index_sequence< S... > )
   {
      ( ( out << get< S >( t ) ), ... );
   }( make_index_sequence< tuple_size_v< tuple< T... > > >{} );

   return out;
}

//-----------------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------------

}
