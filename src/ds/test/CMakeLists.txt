
set( TARGET test_ds )

add_executable( ${TARGET}
   test_ds.cpp
)

target_link_libraries( ${TARGET}
   ds
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )
