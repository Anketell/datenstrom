set( TARGET ds_mysql )
set( MODULE ds_mysql_module )

include_directories( BEFORE, "/usr/include/mysql" )

add_library( ${TARGET} SHARED
   constructor.cpp
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

add_library( ${MODULE} MODULE
   enroll.cpp
)

target_link_libraries( ${MODULE}
   ds_mysql
)

add_subdirectory( test )
