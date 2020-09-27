set( TARGET util )

add_library( ${TARGET} STATIC
   env.cpp
   filesys.cpp
   module.cpp
)

set_property(TARGET ${TARGET} PROPERTY POSITION_INDEPENDENT_CODE ON)

