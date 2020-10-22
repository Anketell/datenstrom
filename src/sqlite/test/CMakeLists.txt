set( TARGET test_ds_sqlite )

add_executable( ${TARGET}
   test_sqlite_db.cpp
   test_sqlite_transaction.cpp
   test_sqlite_parameter.cpp
   test_sqlite_list.cpp
   test_sqlite_blob.cpp
   sqlite_test_data.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )
