#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_mssql_module )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement_enum.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
   types.cpp
)

set( HEADERS
   connection.h
   statement_enum.h
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
   odbc32
)

include( dll_funcs )
module_def( TARGET ${TARGET} )

add_subdirectory( test )
