//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/connect_params.h>
#include <dsutil/url_encode.h>

#include <regex>
#include <functional>
#include <stdexcept>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

typedef std::function< void( connect_params_t & params, const std::string & ) > part_parser_fn;

struct part_def_t
{
   int            index;
   part_parser_fn part_parser;
};

//-----------------------------------------------------------------------------

part_parser_fn path_parser = []( connect_params_t & params, const std::string & value )
{
   static std::regex regex(
      R"(^/.:[^:?&#]+$)",
      std::regex::extended
   );

   if ( std::regex_match( value, regex ) )
      params[ "path" ] = url::unescape( value.substr( 1 ) );
   else
      params[ "path" ] = url::unescape( value );
};

//-----------------------------------------------------------------------------

void parse_param( connect_params_t & params, const std::string & value )
{
   static std::regex regex (
      R"(^(([^=&]+)=([^=&]+)&?)$)",
      std::regex::extended
   );
   std::smatch match_result;

   if ( std::regex_match( value, match_result, regex ) )
      params[ url::unescape( match_result[ 2 ] ) ] = url::unescape( match_result[ 3 ] );
   else
      throw std::invalid_argument( "Bad parameter" );
}

//-----------------------------------------------------------------------------

part_parser_fn part_parser( const std::string & part )
{
   return [ part ]( connect_params_t & params, const std::string & value )
   {
      params[ part ] = url::unescape( value );
   };
}

//-----------------------------------------------------------------------------

part_parser_fn params_parser = []( connect_params_t & params, const std::string & value )
{
  static std::regex regex (
      R"(([^:\/?&#]+&?))",
      std::regex::extended
   );
   std::smatch match_result;

   auto begin = std::sregex_iterator( value.begin(), value.end(), regex );
   auto end   = std::sregex_iterator();

   for ( auto & it = begin; it != end; ++it )
      parse_param( params, it->str() );
};

//-----------------------------------------------------------------------------

connect_params_t parse_connect_string( const std::string & connect_string )
{
   static std::regex regex (
      R"(^(([^:\/?&#]+):)?//([^:?&#\/]*)(:([0-9]+))?(/([^?&#]*))?(\?(([^&#]+)(&[^&#]+)*))?(#(.*))?$)",
      std::regex::extended
   );
   std::smatch match_result;

   static part_def_t part_def[] =
   {
      {  2, part_parser( "type" ) },
      {  3, part_parser( "server" ) },
      {  5, part_parser( "port" ) },
      {  7, path_parser },
      {  9, params_parser },
      { 13, part_parser( "database" ) }
   };

   connect_params_t params;

   if ( std::regex_match( connect_string, match_result, regex ) )
   {
      for ( auto & def : part_def )
      {
         std::string value = match_result[ def.index ];
         if ( !value.empty() )
            def.part_parser( params, value );
      }
   }
   else
      throw std::invalid_argument( "Poorly formed connect string" );

   return params;
}

//-----------------------------------------------------------------------------

std::string form_connect_string( const connect_params_t & connect_params )
{
   connect_params_t params = connect_params;

   std::string type     = url::escape( params[ "type" ] );
   std::string server   = url::escape( params[ "server" ] );
   std::string port     = url::escape( params[ "port" ] );
   std::string path     = url::escape( params[ "path" ] );
   std::string database = url::escape( params[ "database" ] );

   params.erase( "type" );
   params.erase( "server" );
   params.erase( "port" );
   params.erase( "path" );
   params.erase( "database" );

   std::string connect_string = type + "://" + server;

   if ( !port.empty() )
      connect_string += ":" + port;

   if ( !path.empty() )
      connect_string += '/' + path;

   char separator = '?';
   for ( auto it = params.begin(); it != params.end(); separator = '&', it++ )
      connect_string += separator + url::escape( it->first ) + '=' + url::escape( it->second );

   if ( !database.empty() )
      connect_string += '#' + url::escape( database );

   return connect_string;
}

//-----------------------------------------------------------------------------

}
