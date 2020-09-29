set( TARGET util )

add_library( ${TARGET} STATIC
   env.cpp
   filesys.cpp
   module.cpp
)
