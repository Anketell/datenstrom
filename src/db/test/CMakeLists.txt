set( TARGET test_ds_db )

add_executable( ${TARGET}
   test_db.cpp
   test_transaction.cpp
   test_list.cpp
   test_factory.cpp
   test_connect_string.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds
   ds_db
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET})
