//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef TEST_CONTEXT_H
#define TEST_CONTEXT_H

//-----------------------------------------------------------------------------

#include <gtest/gtest.h>

//-----------------------------------------------------------------------------

class Context : public testing::TestWithParam< const char * >
{
protected:

   std::string m_sql_module_path;

public:

   Context( void );
};

//-----------------------------------------------------------------------------

#endif
