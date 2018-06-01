//-----------------------------------------------------------------------------

#include <test_model/object.h>
#include <ds/ds.h>
#include <iostream>

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
             >> o.m_hello;
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
              << o.m_hello;
}

//-----------------------------------------------------------------------------

std::ostream & operator << ( std::ostream & out, const Object & o )
{
   return out << "o.m_i8    = " << ( int )o.m_i8      << std::endl
              << "o.m_i16   = " << o.m_i16            << std::endl
              << "o.m_i32   = " << o.m_i32            << std::endl
              << "o.m_i64   = " << o.m_i64            << std::endl
              << "o.m_u8    = " << ( unsigned )o.m_u8 << std::endl
              << "o.m_u16   = " << o.m_u16            << std::endl
              << "o.m_u32   = " << o.m_u32            << std::endl
              << "o.m_u64   = " << o.m_u64            << std::endl
              << "o.m_f     = " << o.m_f              << std::endl
              << "o.m_d     = " << o.m_d              << std::endl
              << "o.m_hello = " << o.m_hello          << std::endl;
}

//-----------------------------------------------------------------------------
