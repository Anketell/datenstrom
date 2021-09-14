//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#ifndef DS_FACTORY_H
#define DS_FACTORY_H

//-----------------------------------------------------------------------------

#include <map>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

template< typename B, typename K, typename P > class factory
{
   typedef B * ( *Constructor )( P );

   typedef std::map< K, Constructor > ConstructorMap;

   ConstructorMap m_cons_map;

   template< typename D > static B * construct( P param )
   {
      return ::new D( param );
   }

public :

   class Not_found_exception : public std::runtime_error
   {
   public :

      Not_found_exception( void ) :
      std::runtime_error( "Factory key not found" )
      {
      }
   };

   template< typename D > void register_type( K key )
   {
      m_cons_map[ key ] = construct< D >;
   }

   B * operator()( K key, P param ) const
   {
      typename ConstructorMap::const_iterator it = m_cons_map.find( key );
      if ( it == m_cons_map.end() )
         throw Not_found_exception();

      return it->second( param );
   }
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
