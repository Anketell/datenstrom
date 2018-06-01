
set( TARGET ds_sqlite )

add_library( ${TARGET} SHARED
   database.cpp
   statement.cpp
   row.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_db
   sqlite3
)

add_subdirectory( test )
