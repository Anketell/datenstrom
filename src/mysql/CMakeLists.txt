set( TARGET ds_mysql )

include_directories( BEFORE, "/usr/include/mysql" )

add_library( ${TARGET} SHARED
   database.cpp
   statement.cpp
   prepared_statement.cpp
   row.cpp
   prepared_row.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_db
   mariadb
)

add_subdirectory( test )
