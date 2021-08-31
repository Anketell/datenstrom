#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_mysql_module )

find_package(MySQL REQUIRED)

include_directories( BEFORE, ${MYSQL_INCLUDE_DIR} )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
)

set( HEADERS
   connection.h
   statement_base.h
   positional_statement.h
   named_statement.h
   result.h
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

add_subdirectory( test )
