#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_postgresql_module )

if( WIN32 )

   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26451 /wd6011 /wd6385 /wd4244 /wd4267" )

   include_directories( "${NUGET_OUTPUT_DIR}/libpq/native/include" )
   link_directories( "${NUGET_OUTPUT_DIR}/libpq/native/lib/${DS_ARCH}" )

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

if ( WIN32 )

   target_link_libraries( ${TARGET}
      ds_db
      libpq
      ws2_32
   )

   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )

else()

   target_link_libraries( ${TARGET}
      ds_db
      pq
   )

   set_target_properties( ${TARGET} PROPERTIES PREFIX "" )

endif( WIN32 )

if ( DEFINED DS_POSTGRESQL_TEST AND NOT DS_POSTGRESQL_TEST STREQUAL "" )
   add_subdirectory( test )
else()
   message( WARNING "DS_POSTGRESQL_TEST is undefined - PostgeSQL test will not be built" )
endif()
