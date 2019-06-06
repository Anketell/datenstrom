set( TARGET test_ds_db )

add_executable( ${TARGET}
   test_db.cpp
   test_transaction.cpp
   test_list.cpp
   test_factory.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_db
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET})
