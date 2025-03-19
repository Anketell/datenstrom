//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <map>
#include <set>
#include <string>

//-----------------------------------------------------------------------------

namespace ds::db::sql
{

//-----------------------------------------------------------------------------

typedef std::set< std::string > keyset_t;
typedef const char * ( *lookup_t )( const std::string & key );
typedef void ( *keys_t )( keyset_t & keyset );
struct module_t
{
   lookup_t lookup;
   keys_t   keys;
};
typedef std::multimap< std::string, module_t > module_map_t;
typedef void ( * enroll_t )( module_map_t & );

//-----------------------------------------------------------------------------

void enroll_module( module_map_t & module_map, const std::string & path );
void enroll_directory( module_map_t & module_map, const std::string & path );

//-----------------------------------------------------------------------------

}
