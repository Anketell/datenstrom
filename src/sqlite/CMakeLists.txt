set( TARGET ds_sqlite )
set( MODULE ds_sqlite_module )

add_library( ${TARGET} SHARED
   connection.cpp
   statement.cpp
   row.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   sqlite3
)

add_library( ${MODULE} MODULE
   enroll.cpp
   constructor.cpp
)

target_link_libraries( ${MODULE}
   ds_sqlite
)

add_subdirectory( test )
