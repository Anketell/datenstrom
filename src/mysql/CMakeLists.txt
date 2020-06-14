set( TARGET ds_mysql_module )

find_package(MySQL REQUIRED)

include_directories( BEFORE, ${MYSQL_INCLUDE_DIR} )


add_library( ${TARGET} SHARED
enroll.cpp
connection.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   ${MYSQL_LIBRARY}
)

add_subdirectory( test )
