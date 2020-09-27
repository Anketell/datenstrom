set( TARGET ds_db )

add_library( ${TARGET} SHARED
   enroll.cpp
   connection.cpp
   statement.cpp
   result.cpp
   transaction.cpp
   connect_string.cpp
)

target_link_libraries( ${TARGET}
   ds
   util
   dl
)

add_subdirectory( test )
