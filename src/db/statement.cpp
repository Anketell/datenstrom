//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <db/statement.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

statement::statement( std::shared_ptr< impl > impl )
{
   m_impl      = impl;
   m_parameter = 0;
}

//-----------------------------------------------------------------------------

int statement::parameter_count( void ) const
{
   return m_impl->parameter_count();
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int8_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int16_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int32_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( int64_t i )
{
   m_impl->set_parameter( m_parameter++, i );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint8_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint16_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint32_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( uint64_t u )
{
   m_impl->set_parameter( m_parameter++, u );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( double d )
{
   m_impl->set_parameter( m_parameter++, d );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( const char * s )
{
   m_impl->set_parameter( m_parameter++, s );
   return *this;
}

//-----------------------------------------------------------------------------

ds::ostream & statement::operator << ( const std::string & s )
{
   m_impl->set_parameter( m_parameter++, s );
   return *this;
}

//-----------------------------------------------------------------------------

void statement::endr( void )
{
   execute();
}

//-----------------------------------------------------------------------------

void statement::reset( void )
{
   m_impl->reset();
}

//-----------------------------------------------------------------------------

void statement::execute( void )
{
   if ( m_parameter != m_impl->parameter_count() )
      throw std::runtime_error( "Wrong number of parameters" );

   m_parameter = 0;
   m_impl->execute();
}

//-----------------------------------------------------------------------------

rowset statement::result( void )
{
   if ( m_parameter != m_impl->parameter_count() )
      throw std::runtime_error( "Wrong number of parameters" );

   m_parameter = 0;
   return m_impl->result();
}

//-----------------------------------------------------------------------------

statement::iterator statement::begin( void )
{
   return iterator( result() );
}

//-----------------------------------------------------------------------------

statement::iterator statement::end( void )
{
   return iterator();
}

//-----------------------------------------------------------------------------

statement::iterator::iterator( void )
{
}

//-----------------------------------------------------------------------------

statement::iterator::iterator( db::rowset rowset )
{
   m_rowset = rowset;
}

//-----------------------------------------------------------------------------

rowset statement::iterator::operator*( void )
{
   if ( m_rowset.eof() )
      throw std::runtime_error( "No rowset available" );

   return m_rowset;
}

//-----------------------------------------------------------------------------

rowset * statement::iterator::operator->( void )
{
   return &m_rowset;
}

//-----------------------------------------------------------------------------

statement::iterator & statement::iterator::operator++( void )
{
   if ( !m_rowset.step() )
      m_rowset = db::rowset();

   return *this;
}

//-----------------------------------------------------------------------------

statement::iterator & statement::iterator::operator++( int )
{
   return ++*this;
}

//-----------------------------------------------------------------------------

bool statement::iterator::operator==( const iterator &it ) const
{
   return m_rowset == it.m_rowset;
}

//-----------------------------------------------------------------------------

bool statement::iterator::operator!=( const iterator &it ) const
{
   return !( *this == it );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
