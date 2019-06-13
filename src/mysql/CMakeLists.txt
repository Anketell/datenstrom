set( TARGET ds_mysql )

include_directories( BEFORE, "/usr/include/mysql" )

add_library( ${TARGET} SHARED
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   row.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_db
   mariadb
)

add_subdirectory( test )
