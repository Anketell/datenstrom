set( TARGET ds_mssql_module )

add_library( ${TARGET} MODULE
   enroll.cpp
   connection.cpp
#   statement.cpp
#   result.cpp
   error.cpp
#   types.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   odbc32
)

include( dll_funcs )
module_def( TARGET ${TARGET} )

add_subdirectory( test )
