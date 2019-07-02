set( TARGET test_ds_mysql )

add_executable( ${TARGET}
   test_mysql_db.cpp
   test_mysql_list.cpp
   test_mysql_parameter.cpp
   test_mysql_transaction.cpp
   mysql_test_data.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )
