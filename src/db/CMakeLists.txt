set( TARGET ds_db )

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

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
