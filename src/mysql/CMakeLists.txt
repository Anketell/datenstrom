#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_mysql_module )

find_package(MySQL REQUIRED)

include_directories( ${MYSQL_INCLUDE_DIR} )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   rowset.cpp
   error.cpp
)

set( HEADERS
   connection.h
   statement_base.h
   positional_statement.h
   named_statement.h
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
   ${MYSQL_LIBRARY}
)

if ( WIN32 )
   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )
else()
   set_target_properties( ${TARGET} PROPERTIES PREFIX "" )
endif( WIN32 )

add_subdirectory( test )
