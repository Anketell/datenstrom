set( TARGET util )

add_library( ${TARGET} STATIC
   env.cpp
   filesys.cpp
   module.cpp
   string.cpp
   time.cpp
)

target_link_libraries( ${TARGET}
   dl
)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

