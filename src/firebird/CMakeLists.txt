set( TARGET ds_firebird )
set( MODULE ds_firebird_module )

add_library( ${TARGET} SHARED
   transactional.cpp
   connection.cpp
   statements.cpp
   parameter_enum.cpp
   guard.cpp
   statement_base.cpp
   positional_statement.cpp
   named_statement.cpp
   result.cpp
   error.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   fbclient
)

add_library( ${MODULE} MODULE
   enroll.cpp
)

target_link_libraries( ${MODULE}
    ds_firebird
)

add_subdirectory( test )
