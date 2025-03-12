//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <db/context.h>

//-----------------------------------------------------------------------------

struct test_config_t;

//-----------------------------------------------------------------------------

class init_module_paths : public testing::TestWithParam< const test_config_t * >
{
protected:

   const test_config_t * config;

public:

   virtual void SetUp( void ) override;
   virtual void TearDown( void ) override;
};

//-----------------------------------------------------------------------------
