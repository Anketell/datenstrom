//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <dsutil/connect_params.h>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

class impl;

//-----------------------------------------------------------------------------

template< class T > struct factory_helper
{
   static constexpr const char * TYPE = T::TYPE;
   static impl * construct( const connect_params_t & params )
   {
      throw std::runtime_error( params[ "type" ] + " constructor not implemented" );
   }
};

//-----------------------------------------------------------------------------

}
