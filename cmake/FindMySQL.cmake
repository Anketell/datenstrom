# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIR - where to find mysql.h, etc.
#  MYSQL_LIBRARIES   - List of libraries when using MySQL.
#  MYSQL_FOUND       - True if MySQL found.

IF (MYSQL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(MYSQL_FIND_QUIETLY TRUE)
ENDIF (MYSQL_INCLUDE_DIR)

if (UNIX)

  FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
    /usr/local/include/mysql
    /usr/include/mysql
  )

  SET(MYSQL_NAMES mysqlclient mysqlclient_r)
  FIND_LIBRARY(MYSQL_LIBRARY
    NAMES ${MYSQL_NAMES}
    PATHS /usr/lib /usr/local/lib
    PATH_SUFFIXES mysql
  )

ELSEIF(MSVC)

  if ( "${DS_ARCH}" STREQUAL "x64" )

    FILE( GLOB MARIADB_DIR "$ENV{PROGRAMFILES}/MariaDB*" )

    FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
      PATHS "${MARIADB_DIR}" "${MARIADB_DIR}/MariaDB/MariaDB Connector C"
      PATH_SUFFIXES include/mysql
    )

    SET(MYSQL_NAMES libmariadb.lib )
    FIND_LIBRARY(MYSQL_LIBRARY
      NAMES ${MYSQL_NAMES}
      PATHS  "${MARIADB_DIR}" "${MARIADB_DIR}/MariaDB/MariaDB Connector C"
      PATH_SUFFIXES lib
      NO_DEFAULT_PATH
    )
  ELSE()

    FILE( GLOB MARIADB_DIR "$ENV{PROGRAMFILES} (x86)/MariaDB*" )

    FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
      PATHS "${MARIADB_DIR}" "${MARIADB_DIR}/MariaDB Connector C"
      PATH_SUFFIXES include inlcude/mysql
    )

    SET(MYSQL_NAMES libmariadb.lib )
    FIND_LIBRARY(MYSQL_LIBRARY
      NAMES ${MYSQL_NAMES}
      PATHS "${MARIADB_DIR}" "${MARIADB_DIR}/MariaDB Connector C"
      PATH_SUFFIXES lib
      NO_DEFAULT_PATH
    )

  endif()

endif( UNIX)

IF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  SET(MYSQL_FOUND TRUE)
  SET( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )
ELSE (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  SET(MYSQL_FOUND FALSE)
  SET( MYSQL_LIBRARIES )
ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)

IF (MYSQL_FOUND)
  IF (NOT MYSQL_FIND_QUIETLY)
    MESSAGE(STATUS "Found MySQL: ${MYSQL_LIBRARY}")
  ENDIF (NOT MYSQL_FIND_QUIETLY)
ELSE (MYSQL_FOUND)
  IF (MYSQL_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for MySQL libraries named ${MYSQL_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find MySQL library")
  ENDIF (MYSQL_FIND_REQUIRED)
ENDIF (MYSQL_FOUND)

MARK_AS_ADVANCED(
  MYSQL_LIBRARY
  MYSQL_INCLUDE_DIR
)

