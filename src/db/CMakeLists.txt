add_library( ds_db SHARED
   database.cpp
   statement.cpp
   row.cpp
   transaction.cpp
)

add_subdirectory( test )
