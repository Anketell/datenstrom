set( TARGET ds_mysql )
set( MODULE ds_mysql_module )

include_directories( BEFORE, "/usr/include/mysql" )

add_library( ${TARGET} SHARED
   connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   mariadb
)

add_library( ${MODULE} MODULE
   enroll.cpp
   impl_traits.cpp
)

target_link_libraries( ${MODULE}
   ds_mysql
)

add_subdirectory( test )
