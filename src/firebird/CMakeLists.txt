set( TARGET ds_firebird_module )

if( WIN32 )

   include( firebird )

   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd26451 /wd6011 /wd6385 /wd4244 /wd4267" )

endif( WIN32 )

add_library( ${TARGET} MODULE
   enroll.cpp
   transactional.cpp
   connection.cpp
   statement_enum.cpp
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
   util
   fbclient
)

if ( WIN32 )
   include( dll_funcs )
   module_def( TARGET ${TARGET} )
endif( WIN32 )

add_subdirectory( test )
