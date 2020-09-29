
set( TARGET test_ds )

add_executable( ${TARGET}
   test_ds.cpp
   test_factory.cpp
)

if ( WIN32 )
   add_dependencies( ${TARGET} googletest )
endif ( WIN32 )

target_link_libraries( ${TARGET}
   ds
   gtest
   gtest_main
)

add_test( ${TARGET} ${TARGET} )
