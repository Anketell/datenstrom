set( TARGET test_ds_mssql )

add_executable( ${TARGET}
   test_mssql_db.cpp
   test_mssql_transaction.cpp
#   test_mssql_parameter.cpp
   test_mssql_list.cpp
   mssql_test_data.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_dependencies( ${TARGET} ds_mssql_module )

add_test( ${TARGET} ${TARGET} )
