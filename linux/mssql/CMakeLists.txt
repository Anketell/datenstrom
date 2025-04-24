#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_mssql_module )

set( SOURCE
   enroll.cpp
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   rowset.cpp
   error.cpp
   types.cpp
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

add_library( mssql_stmt_enum STATIC
   statement_enum.cpp
   statement_enum.h
)

add_library( ${TARGET} MODULE
   ${SOURCE}
   ${HEADERS}
)

set_target_properties( ${TARGET} PROPERTIES PREFIX "" )

set( ODBC_LIBRARY odbc )

target_link_libraries( ${TARGET}
   mssql_stmt_enum
   ds_db
   ${ODBC_LIBRARY}
)

target_compile_options( mssql_stmt_enum PRIVATE "-fPIC" )

add_subdirectory( test )
