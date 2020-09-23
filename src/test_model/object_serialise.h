//-----------------------------------------------------------------------------

#ifndef OBJECT_SERIALISE_H
#define OBJECT_SERIALISE_H

//-----------------------------------------------------------------------------

#include <iosfwd>

//-----------------------------------------------------------------------------

namespace ds
{
   class istream;
   class ostream;
}

//-----------------------------------------------------------------------------

class Object;
class Object_alt;

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object & o );
ds::ostream & operator << ( ds::ostream & out, const Object & o );

//-----------------------------------------------------------------------------

std::ostream & operator << ( std::ostream & out, const Object & o );

//-----------------------------------------------------------------------------

ds::istream & operator >> ( ds::istream & in, Object_alt & o );
ds::ostream & operator << ( ds::ostream & out, const Object_alt & o );

//-----------------------------------------------------------------------------

std::ostream & operator << ( std::ostream & out, const Object_alt & o );

//-----------------------------------------------------------------------------

#endif
