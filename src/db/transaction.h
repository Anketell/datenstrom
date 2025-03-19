//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds::db
{

//-----------------------------------------------------------------------------

class transactional;
class impl;

//-----------------------------------------------------------------------------

class transaction
{
   transactional & m_db;
   int             m_uncaught_count;

public:

   transaction( transactional & db );
   ~transaction( void );
};

//-----------------------------------------------------------------------------

class savepoint
{
   impl      & m_db;
   std::string m_name;
   int         m_uncaught_count;

public:

   savepoint( impl & db, const std::string & name );
   ~savepoint( void );
};

//-----------------------------------------------------------------------------

}

