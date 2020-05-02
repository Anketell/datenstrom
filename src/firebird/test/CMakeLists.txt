set( TARGET test_ds_firebird )

add_executable( ${TARGET}
   test_firebird_db.cpp
   test_firebird_list.cpp
   test_firebird_parameter.cpp
   test_firebird_transaction.cpp
   firebird_test_data.cpp
)

target_link_libraries( ${TARGET}
   ds_db
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )
