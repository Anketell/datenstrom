#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

include_directories( BEFORE, ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( ds )

if ( DS_CSV )
   add_subdirectory( csv )
endif()

if ( DS_DB )

   add_subdirectory( db )

   if ( DS_FIREBIRD )
      add_subdirectory( firebird )
   endif()

   if ( DS_MYSQL )
      add_subdirectory( mysql )
   endif()

   if ( DS_SQLITE )
      add_subdirectory( sqlite )
   endif()

endif()

add_subdirectory( util )
add_subdirectory( test_model )
