//-----------------------------------------------------------------------------
//
// Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
//
//-----------------------------------------------------------------------------

#include <dsutil/connect_params.h>

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
      params[ "path" ] = value.substr( 1 );
   else
      params[ "path" ] = value;
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
      params[ match_result[ 2 ] ] = match_result[ 3 ];
   else
      throw std::invalid_argument( "Bad parameter" );
}

//-----------------------------------------------------------------------------

part_parser_fn part_parser( const std::string & part )
{
   return [ part ]( connect_params_t & params, const std::string & value )
   {
      params[ part ] = value;
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

}
