set( TARGET ds_sqlite_module )

add_library( ${TARGET} MODULE
   enroll.cpp
   connection.cpp
   statement.cpp
   result.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   sqlite3
)

add_subdirectory( test )
