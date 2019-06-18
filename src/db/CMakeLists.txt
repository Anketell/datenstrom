set( TARGET ds_db )

add_library( ${TARGET} SHARED
   enroll.cpp
   connection.cpp
   statement.cpp
   row.cpp
   transaction.cpp
   connect_string.cpp
)

target_link_libraries( ${TARGET}
   ds
   dl
)

add_subdirectory( test )
