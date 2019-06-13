add_library( ds_db SHARED
   connection.cpp
   statement.cpp
   row.cpp
   transaction.cpp
   factory.cpp
)

add_subdirectory( test )
