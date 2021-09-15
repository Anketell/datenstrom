#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET sqlite3 )

set( SQLITE3_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories( ${SQLITE3_SRC_DIR} )

add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_JSON1)
add_definitions(-DSQLITE_ENABLE_RBU)
add_definitions(-DSQLITE_ENABLE_STAT4)

set( SOURCE
   ${SQLITE3_SRC_DIR}/sqlite3.c
)

set( HEADERS
   ${SQLITE3_SRC_DIR}/sqlite3.h
)

add_library( ${TARGET} STATIC
   ${SOURCE}
   ${HEADERS}
 )
