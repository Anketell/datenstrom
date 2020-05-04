//-----------------------------------------------------------------------------

#ifndef DS_FIREBIRD_PARAMETERS_H
#define DS_FIREBIRD_PARAMETERS_H

//-----------------------------------------------------------------------------

#include <string>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace firebird
{

//-----------------------------------------------------------------------------

class parameter_enum
{
   const std::string & m_sql;

public:

   parameter_enum( const std::string & sql );

   class iterator
   {
   public:

      struct parameter_t
      {
         uint32_t from = 0;
         uint32_t len  = 0;
      };

   private:

      const std::string & m_sql;
      parameter_t         m_parameter;

      void next_parameter( void );

   public:

      iterator( const std::string & sql = "" );

      const parameter_t & operator*( void ) const;
      const parameter_t * operator->( void ) const;

      iterator operator++( void );
      iterator operator++( int );

      bool operator==( const iterator & it ) const;
      bool operator!=( const iterator & it ) const;
   };

   iterator begin( void ) const;
   iterator end( void ) const;
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

namespace std
{
template<> class iterator_traits< ds::firebird::parameter_enum::iterator >
{
public:

   typedef int32_t                                difference_type;
   typedef ds::firebird::parameter_enum::iterator value_type;
   typedef value_type *                           pointer;
   typedef value_type &                           reference;
   typedef forward_iterator_tag                   iterator_category;
};
}

//-----------------------------------------------------------------------------

#endif
