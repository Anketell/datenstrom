//-----------------------------------------------------------------------------

#include <db/factory.h>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace db
{

//-----------------------------------------------------------------------------

db::connect_params_t factory::parse_connection( const std::string & connection )
{
   db::connect_params_t params;

   std::string::size_type pos = connection.find( ":" );

   if ( pos == std::string::npos )
      throw std::runtime_error( "Poorly formed connection string: " + connection );

   params[ "type" ] = connection.substr( 0, pos );
   params[ "path" ] = connection.substr( pos + 1 );

   return params;
}

//-----------------------------------------------------------------------------

impl * factory::operator()( std::string connection ) const
{
   db::connect_params_t params = parse_connection( connection );

   return m_ds_factory( params[ "type" ], params );
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
