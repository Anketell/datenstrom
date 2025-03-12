//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/test/init_module_paths.h>
#include <db/test/test_mod_config.h>
#include <db/context.h>

//-----------------------------------------------------------------------------

void init_module_paths::SetUp( void )
{
   config = GetParam();

   ds::db::connection::enroll_db_path_list( config->ds_module_path );
   ds::db::context::enroll_sql_path_list( config->sql_module_path );
}

//-----------------------------------------------------------------------------

void init_module_paths::TearDown( void )
{
   ds::db::context::clear_sql_path_list();
   ds::db::connection::clear_db_path_list();
}

//-----------------------------------------------------------------------------
