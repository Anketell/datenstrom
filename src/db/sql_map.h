//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_DB_SQL_MAP_H
#define DS_DB_SQL_MAP_H

//-----------------------------------------------------------------------------

#include <map>
#include <string>

//-----------------------------------------------------------------------------

namespace ds::db::sql
{

//-----------------------------------------------------------------------------

typedef const char * ( *lookup_t )( const std::string & key );
typedef std::multimap< std::string, lookup_t > map_t;
typedef void ( * enroll_t )( map_t & );

//-----------------------------------------------------------------------------

void enroll_module( map_t & sql_map, const std::string & path );
void enroll_directory( map_t & sql_map, const std::string & path );

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
