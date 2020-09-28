
set( TARGET test_ds )

add_executable( ${TARGET}
   test_ds.cpp
   test_factory.cpp
)

target_link_libraries( ${TARGET}
   ds
   gtestd
   gtest_maind
)

add_test( ${TARGET} ${TARGET} )
