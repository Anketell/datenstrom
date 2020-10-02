set( TARGET ds_csv )

# set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library( ${TARGET} SHARED
   istream.cpp
   ostream.cpp
)

target_link_libraries( ${TARGET}
   ds
)

if( WIN32 )

   set( DATA
      ??_R0?AVistream@csv@ds@@@8
      ??_R0?AVostream@csv@ds@@@8
   )

   set( FUNCTIONS
      ??0istream@csv@ds@@QEAA@AEAV?$basic_istream@DU?$char_traits@D@std@@@std@@@Z
      ??0ostream@csv@ds@@QEAA@AEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@@Z
   )

   include( dll_funcs )
   dll_export( TARGET "${TARGET}" DATA "${DATA}" FUNCTIONS "${FUNCTIONS}" )

endif( WIN32 )


add_subdirectory( test )
