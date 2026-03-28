#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_sqlite_module )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement.cpp
   rowset.cpp
   error.cpp
)

set( HEADERS
   connection.h
   statement.h
   rowset.h
   error.h
   types.h
)

add_library( ${TARGET} MODULE
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds_db
   sqlite3
)

if ( WIN32 )
   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )
else()
   set_target_properties( ${TARGET} PROPERTIES PREFIX "" )
endif( WIN32 )

if( DEFINED DS_SQLITE_TEST AND NOT DS_SQLITE_TEST STREQUAL "" )
   add_subdirectory( test )
else()
   message( WARNING "DS_SQLITE_TEST is undefined - SQLite test will not be built" )
endif()

