set( TARGET test_ds_csv )

add_executable( ${TARGET}
   test_csv.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds
   ds_csv
   test_model
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )