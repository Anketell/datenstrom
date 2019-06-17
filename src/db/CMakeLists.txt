add_library( ds_db SHARED
   connection.cpp
   statement.cpp
   row.cpp
   transaction.cpp
   connect_string.cpp
)

add_subdirectory( test )
