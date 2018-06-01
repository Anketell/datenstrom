add_library( ds_db SHARED
   statement.cpp
   row.cpp
   transaction.cpp
)

add_subdirectory( test )
