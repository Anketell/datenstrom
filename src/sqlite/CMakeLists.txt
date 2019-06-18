set( TARGET ds_sqlite )
set( MODULE ds_sqlite_module )

add_library( ${TARGET} SHARED
   constructor.cpp
   connection.cpp
   statement.cpp
   row.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_db
   sqlite3
)

add_library( ${MODULE} MODULE
   enroll.cpp
)

target_link_libraries( ${MODULE}
   ds_sqlite
)

add_subdirectory( test )
