//-----------------------------------------------------------------------------

#include <csv/csv.h>
#include <istream>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace csv
{

//-----------------------------------------------------------------------------

Field_separator_expected::Field_separator_expected( void ) :
std::runtime_error( "Field separator expected" )
{
}

//-----------------------------------------------------------------------------

Unterminated_quotes::Unterminated_quotes( void ) :
std::runtime_error( "Unterminated quotes" )
{
}

//-----------------------------------------------------------------------------

Quotes_not_allowed::Quotes_not_allowed( void ) :
std::runtime_error( "Quotes not allowed" )
{
}

//-----------------------------------------------------------------------------

static void skip_ws( std::istream & in )
{
   int c = in.peek();
   while ( c == ' ' || c == '\t' )
   {
      in.get();
      c = in.peek();
   }
}

//-----------------------------------------------------------------------------

 void get_field_separator( std::istream & in )
{
   if ( !in.eof() )
   {
      int c = in.peek();
      switch ( c )
      {
         case ',':
            in.get();
            break;

         case '\n':
         case '\r':
         case '\x1a':
            break;

         default:
            throw Field_separator_expected();
      }
   }
}

//-----------------------------------------------------------------------------

static bool quoted_string( std::istream & in )
{
   int c = in.peek();

   if ( c != '\"' )
      return false;

   in.get();

   return true;
}

//-----------------------------------------------------------------------------

static std::string get_quoted_string( std::istream & in )
{
   std::string s;

   while ( !in.eof() )
   {
      int c = in.peek();

      switch ( c )
      {
         case '\r':
         case '\n':
         case '\x1a':
            goto unterminated_quotes;

         case '\"':
            in.get();
            if ( in.peek() != '\"'  )
               return s;
            break;
      }
      s += static_cast< char >( c );
      in.get();
   }

unterminated_quotes:

   throw Unterminated_quotes();
}

//-----------------------------------------------------------------------------

static std::string get_unquoted_string( std::istream & in )
{
   std::string s;

   while ( !in.eof() )
   {
      int c = in.peek();

      switch ( c )
      {
         case ',':
         case '\r':
         case '\n':
         case '\x1a':
            goto end_of_field;

         case '\"':
            throw Quotes_not_allowed();
      }
      s += static_cast< char >( c );
      in.get();
   }

end_of_field:

   return s;
}

//-----------------------------------------------------------------------------

static std::string get_string( std::istream & in )
{
   if ( quoted_string( in ) )
      return get_quoted_string( in );
   else
      return get_unquoted_string( in );
}

//-----------------------------------------------------------------------------

istream::istream( std::istream & in ) :
m_in( in )
{
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int8_t & i )
{
   int16_t i16;

   skip_ws( m_in );
   m_in >> i16;
   i = i16;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int16_t & i )
{
   skip_ws( m_in );
   m_in >> i;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int32_t & i )
{
   skip_ws( m_in );
   m_in >> i;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( int64_t & i )
{
   skip_ws( m_in );
   m_in >> i;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint8_t & u )
{
   uint16_t u16;

   skip_ws( m_in );
   m_in >> u16;
   u = u16;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint16_t & u )
{
   skip_ws( m_in );
   m_in >> u;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint32_t & u )
{
   skip_ws( m_in );
   m_in >> u;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( uint64_t & u )
{
   skip_ws( m_in );
   m_in >> u;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( double & d )
{
   skip_ws( m_in );
   m_in >> d;
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

ds::istream & istream::operator >> ( std::string & s )
{
   skip_ws( m_in );
   s = get_string( m_in );
   skip_ws( m_in );
   get_field_separator( m_in );

   return *this;
}

//-----------------------------------------------------------------------------

void istream::endr( void )
{
   while ( !m_in.eof() )
   {
      int c = m_in.peek();
      if ( c == '\r' || c == '\n' )
         break;

         m_in.get();
   }

   while ( !m_in.eof() )
   {
      int c = m_in.peek();
      if ( c != '\r' && c != '\n' )
         break;

         m_in.get();
   }
}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}
