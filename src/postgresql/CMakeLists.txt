#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_postgresql_module )

if( WIN32 )

   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26451 /wd6011 /wd6385 /wd4244 /wd4267" )

   include_directories( "${NUGET_OUTPUT_DIR}/libpq/native/include" )
   link_directories( "${NUGET_OUTPUT_DIR}/libpq/native/lib/x64" )

endif( WIN32 )

set( SOURCE
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   rowset.cpp
   enroll.cpp
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

add_library( ${TARGET} MODULE
   ${SOURCE}
   ${HEADERS}
)

target_link_libraries( ${TARGET}
   ds_db
   pq
)

if ( WIN32 )

   target_link_libraries( ${TARGET}
      ws2_32
   )

#   target_link_options( ${TARGET} PRIVATE "/NODEFAULTLIB:msvcrtd.lib" )
   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )
else()
   set_target_properties( ${TARGET} PROPERTIES PREFIX "" )
endif( WIN32 )

if ( DEFINED DS_POSTGRESQL_TEST AND NOT DS_POSTGRESQL_TEST STREQUAL "" )
   add_subdirectory( test )
else()
   message( WARNING "DS_POSTGRESQL_TEST is undefined - PostgeSQL test will not be built" )
endif()
