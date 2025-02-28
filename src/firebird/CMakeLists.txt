#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET ds_firebird_module )

if( WIN32 )

   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26451 /wd6011 /wd6385 /wd4244 /wd4267" )

endif( WIN32 )

if( UNIX )

   include_directories( "/usr/include/firebird" )

endif( UNIX )

set( SOURCE
   enroll.cpp
   transactional.cpp
   connection.cpp
   statement_enum.cpp
   guard.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   rowset.cpp
   error.cpp
   types.cpp
)

set( HEADERS
   transactional.h
   connection.h
   statement_enum.h
   guard.h
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

if( DEFINED ENV{DS_FB_INC_PATH} )

   target_include_directories( ${TARGET} PUBLIC $ENV{DS_FB_INC_PATH} )

endif()


if( DEFINED ENV{DS_FB_LIB_PATH} )

   target_link_directories( ${TARGET} PUBLIC $ENV{DS_FB_LIB_PATH} )

endif()

if( DEFINED ENV{DS_FB_CLIENT_LIB} )

   set( FB_CLIENT_LIB $ENV{DS_FB_CLIENT_LIB} )

else()

   set( FB_CLIENT_LIB fbclient)

endif()

target_link_libraries( ${TARGET}
   ds_db
   ${FB_CLIENT_LIB}
)

if ( WIN32 )
   include( dll_funcs )
   db_module_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
