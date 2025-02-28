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

add_subdirectory( test )

if ( WIN32 )
   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )
endif( WIN32 )
