//-----------------------------------------------------------------------------

#ifndef DS_CSV_H
#define DS_CSV_H

//-----------------------------------------------------------------------------

#include <ds/ds.h>
#include <iosfwd>

//-----------------------------------------------------------------------------

namespace ds
{

//-----------------------------------------------------------------------------

namespace csv
{

//-----------------------------------------------------------------------------

class istream : public ds::istream
{
   std::istream & m_in;

public:

   istream( std::istream & in );

   virtual ds::istream & operator >> ( int8_t & ) override;
   virtual ds::istream & operator >> ( int16_t & ) override;
   virtual ds::istream & operator >> ( int32_t & ) override;
   virtual ds::istream & operator >> ( int64_t & ) override;

   virtual ds::istream & operator >> ( uint8_t & ) override;
   virtual ds::istream & operator >> ( uint16_t & ) override;
   virtual ds::istream & operator >> ( uint32_t & ) override;
   virtual ds::istream & operator >> ( uint64_t & ) override;

   virtual ds::istream & operator >> ( std::string & ) override;

   virtual ds::istream & operator >> ( double & ) override;

   virtual void endr( void );
};

//-----------------------------------------------------------------------------

class ostream : public ds::ostream
{
   std::ostream & m_out;

public:

   ostream( std::ostream & out );

   virtual ds::ostream & operator << ( int8_t ) override;
   virtual ds::ostream & operator << ( int16_t ) override;
   virtual ds::ostream & operator << ( int32_t ) override;
   virtual ds::ostream & operator << ( int64_t ) override;

   virtual ds::ostream & operator << ( uint8_t ) override;
   virtual ds::ostream & operator << ( uint16_t ) override;
   virtual ds::ostream & operator << ( uint32_t ) override;
   virtual ds::ostream & operator << ( uint64_t ) override;

   virtual ds::ostream & operator << ( double ) override;

   virtual ds::ostream & operator << ( const char * ) override;
   virtual ds::ostream & operator << ( const std::string & ) override;

   virtual void endr( void );
};

//-----------------------------------------------------------------------------

class Field_separator_expected : public std::runtime_error
{
public:

   Field_separator_expected( void );
};

//-----------------------------------------------------------------------------

class Unterminated_quotes : public std::runtime_error
{
public:

   Unterminated_quotes( void );
};

//-----------------------------------------------------------------------------

class Quotes_not_allowed : public std::runtime_error
{
public:

   Quotes_not_allowed( void );
};

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

}

//-----------------------------------------------------------------------------

#endif
