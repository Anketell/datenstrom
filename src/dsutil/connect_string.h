//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <dsutil/connect_params.h>

//-----------------------------------------------------------------------------

namespace ds
{
   connect_params_t parse_connect_string( const std::string     & connect_string );
   std::string      form_connect_string( const connect_params_t & connect_params );
}

//-----------------------------------------------------------------------------
