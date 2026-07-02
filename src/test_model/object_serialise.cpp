//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <test_model/object.h>
#include <std/optional_serialise.h>
#include <ds/ds.h>
#include <iostream>

//-----------------------------------------------------------------------------

namespace test
{

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object & o )
{
   return in >> o.m_i8
             >> o.m_i16
             >> o.m_i32
             >> o.m_i64
             >> o.m_u8
             >> o.m_u16
             >> o.m_u32
             >> o.m_u64
             >> o.m_f
             >> o.m_d
             >> o.m_hello
             >> o.m_date
             >> o.m_time
             >> o.m_datetime;
}

//-----------------------------------------------------------------------------

ds::ostream & operator << ( ds::ostream & out, const Object & o )
{
   return out << o.m_i8
              << o.m_i16
              << o.m_i32
              << o.m_i64
              << o.m_u8
              << o.m_u16
              << o.m_u32
              << o.m_u64
              << o.m_f
              << o.m_d
              << o.m_hello
              << o.m_date
              << o.m_time
              << o.m_datetime;
}

//-----------------------------------------------------------------------------

std::ostream & operator << ( std::ostream & out, const Object & o )
{
   return out << "o.m_i8         = " << ( int )o.m_i8      << std::endl
              << "o.m_i16        = " << o.m_i16            << std::endl
              << "o.m_i32        = " << o.m_i32            << std::endl
              << "o.m_i64        = " << o.m_i64            << std::endl
              << "o.m_u8         = " << ( unsigned )o.m_u8 << std::endl
              << "o.m_u16        = " << o.m_u16            << std::endl
              << "o.m_u32        = " << o.m_u32            << std::endl
              << "o.m_u64        = " << o.m_u64            << std::endl
              << "o.m_f          = " << o.m_f              << std::endl
              << "o.m_d          = " << o.m_d              << std::endl
              << "o.m_hello      = " << o.m_hello          << std::endl
              << "o.m_date       = " << o.m_date           << std::endl
              << "o.m_time       = " << o.m_time           << std::endl
              << "o.m_datetime   = " << o.m_datetime       << std::endl;
}

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object_alt & o )
{
   return in >> o.m_i8
             >> o.m_i16
             >> o.m_i32
             >> o.m_i64
             >> o.m_u8
             >> o.m_u16
             >> o.m_u32
             >> o.m_u64
             >> o.m_f
             >> o.m_d
             >> o.m_hello
             >> o.m_unix_date
             >> o.m_unix_time
             >> o.m_unix_datetime;
}

//-----------------------------------------------------------------------------

ds::ostream & operator << ( ds::ostream & out, const Object_alt & o )
{
   return out << o.m_i8
              << o.m_i16
              << o.m_i32
              << o.m_i64
              << o.m_u8
              << o.m_u16
              << o.m_u32
              << o.m_u64
              << o.m_f
              << o.m_d
              << o.m_hello
              << o.m_unix_date
              << o.m_unix_time
              << o.m_unix_datetime;
}

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object_opt & o )
{
   return in >> o.m_i8
             >> o.m_i16
             >> o.m_i32
             >> o.m_i64
             >> o.m_u8
             >> o.m_u16
             >> o.m_u32
             >> o.m_u64
             >> o.m_f
             >> o.m_d
             >> o.m_hello
             >> o.m_date
             >> o.m_time
             >> o.m_datetime;  
}

//-----------------------------------------------------------------------------

ds::ostream & operator << ( ds::ostream & out, const Object_opt & o )
{
   return out << o.m_i8
              << o.m_i16
              << o.m_i32
              << o.m_i64
              << o.m_u8
              << o.m_u16
              << o.m_u32
              << o.m_u64
              << o.m_f
              << o.m_d
              << o.m_hello
              << o.m_date
              << o.m_time
              << o.m_datetime;
}

//-----------------------------------------------------------------------------

std::ostream & operator << ( std::ostream & out, const Object_alt & o )
{
   return out << "o.m_i8            = " << ( int )o.m_i8      << std::endl
              << "o.m_i16           = " << o.m_i16            << std::endl
              << "o.m_i32           = " << o.m_i32            << std::endl
              << "o.m_i64           = " << o.m_i64            << std::endl
              << "o.m_u8            = " << ( unsigned )o.m_u8 << std::endl
              << "o.m_u16           = " << o.m_u16            << std::endl
              << "o.m_u32           = " << o.m_u32            << std::endl
              << "o.m_u64           = " << o.m_u64            << std::endl
              << "o.m_f             = " << o.m_f              << std::endl
              << "o.m_d             = " << o.m_d              << std::endl
              << "o.m_hello         = " << o.m_hello          << std::endl
              << "o.m_unix_date     = " << o.m_unix_date      << std::endl
              << "o.m_unix_time     = " << o.m_unix_time      << std::endl
              << "o.m_unix_datetime = " << o.m_unix_datetime  << std::endl;
}

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object_list_t & l )
{
   while ( !in.eof() )
   {
      l.push_back( in );
      in >> ds::endr;
   }

   return in;
}

//-----------------------------------------------------------------------------

}

