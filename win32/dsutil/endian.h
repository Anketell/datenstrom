//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <cstdint>

//-----------------------------------------------------------------------------

namespace ds::endian
{

//-----------------------------------------------------------------------------

uint16_t htol( uint16_t u );
uint32_t htol( uint32_t u );
uint64_t htol( uint64_t u );

int16_t htol( int16_t i );
int32_t htol( int32_t i );
int64_t htol( int64_t i );

//-----------------------------------------------------------------------------

uint16_t ltoh( uint16_t u );
uint32_t ltoh( uint32_t u );
uint64_t ltoh( uint64_t u );

int16_t ltoh( int16_t i );
int32_t ltoh( int32_t i );
int64_t ltoh( int64_t i );

//-----------------------------------------------------------------------------

uint16_t htob( uint16_t u );
uint32_t htob( uint32_t u );
uint64_t htob( uint64_t u );

int16_t htob( int16_t i );
int32_t htob( int32_t i );
int64_t htob( int64_t i );

//-----------------------------------------------------------------------------

uint16_t btoh( uint16_t u );
uint32_t btoh( uint32_t u );
uint64_t btoh( uint64_t u );

int16_t btoh( int16_t i );
int32_t btoh( int32_t i );
int64_t btoh( int64_t i );

//-----------------------------------------------------------------------------

}
