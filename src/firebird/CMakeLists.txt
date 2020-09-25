set( TARGET ds_firebird_module )

add_library( ${TARGET} MODULE
   enroll.cpp
   transactional.cpp
   connection.cpp
   statement_enum.cpp
   parameter_enum.cpp
   guard.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
   types.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   fbclient
)

add_subdirectory( test )
