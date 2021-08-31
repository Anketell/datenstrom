#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_sqlite_module )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement.cpp
   result.cpp
   error.cpp
)

set( HEADERS
   connection.h
   statement.h
   result.h
   error.h
   types.h
)

add_library( ${TARGET} MODULE
   ${SOURCES}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds_db
   sqlite3
)

add_subdirectory( test )
