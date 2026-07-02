//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef OBJECT_H
#define OBJECT_H

//-----------------------------------------------------------------------------

#include <string>
#include <cstdint>
#include <vector>
#include <optional>

//-----------------------------------------------------------------------------

namespace test
{

//-----------------------------------------------------------------------------

struct Object
{
   int8_t      m_i8;
   int16_t     m_i16;
   int32_t     m_i32;
   int64_t     m_i64;
   uint8_t     m_u8;
   uint16_t    m_u16;
   uint32_t    m_u32;
   uint64_t    m_u64;
   double      m_f;
   double      m_d;
   std::string m_hello;
   std::string m_date;
   std::string m_time;
   std::string m_datetime;

   bool operator==( const  Object & o ) const;
};

//-----------------------------------------------------------------------------

struct Object_alt
{
   int8_t      m_i8;
   int16_t     m_i16;
   int32_t     m_i32;
   int64_t     m_i64;
   uint8_t     m_u8;
   uint16_t    m_u16;
   uint32_t    m_u32;
   uint64_t    m_u64;
   double      m_f;
   double      m_d;
   std::string m_hello;
   int64_t     m_unix_date;
   int64_t     m_unix_time;
   int64_t     m_unix_datetime;

   bool operator==( const Object_alt & o ) const;
};

//-----------------------------------------------------------------------------

struct Object_opt
{
   std::optional< int8_t      > m_i8;
   std::optional< int16_t     > m_i16;
   std::optional< int32_t     > m_i32;
   std::optional< int64_t     > m_i64;
   std::optional< uint8_t     > m_u8;
   std::optional< uint16_t    > m_u16;
   std::optional< uint32_t    > m_u32;
   std::optional< uint64_t    > m_u64;
   std::optional< double      > m_f;
   std::optional< double      > m_d;
   std::optional< std::string > m_hello;
   std::optional< std::string > m_date;
   std::optional< std::string > m_time;
   std::optional< std::string > m_datetime;

   bool operator==( const Object_opt & o ) const;
};

//-----------------------------------------------------------------------------

typedef std::vector< Object > Object_list_t;

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

extern test::Object     data[ 2 ];
extern test::Object_alt data_alt[ 2 ];
extern test::Object     data_hires[ 2 ];

//-----------------------------------------------------------------------------

#endif
