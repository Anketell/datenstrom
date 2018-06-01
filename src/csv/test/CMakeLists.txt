set( TARGET test_ds_csv )

add_executable( ${TARGET}
   test_csv.cpp
)

target_link_libraries( ${TARGET}
   ds
   ds_csv
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )