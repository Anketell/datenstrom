#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_db )

set( SOURCE
   enroll.cpp
   connection.cpp
   context.cpp
   statement.cpp
   rowset.cpp
   simple_result.cpp
   transaction.cpp
   parameter.cpp
   sql_map.cpp
)

set( HEADERS
   enroll.h
   connection.h
   context.h
   statement.h
   rowset.h
   simple_result.h
   transaction.h
   factory.h
   factory_helper.h
   impl.h
   transactional.h
   parameter.h
   sql_map.h
)

add_library( ${TARGET} SHARED
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds
   ds_util
)

set( SQL_MODULE_LIB sql_module_harness )

set( SQL_MODULE_LIB_SRC
   sql_module_harness.cpp
)

set( SQL_MODULE_LIB_INC
   sql_module_harness.h
)

add_library( ${SQL_MODULE_LIB} STATIC
   ${SQL_MODULE_LIB_SRC}
   ${SQL_MODULE_LIB_INC}
)

if ( WIN32 )
   include( dll_funcs )
   dll_def( TARGET ${TARGET} )
else ( WIN32 )

   target_compile_options( ${SQL_MODULE_LIB} PRIVATE "-fPIC" )

endif( WIN32 )

add_subdirectory( test )
