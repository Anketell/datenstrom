set( TARGET win32util )

add_library( ${TARGET} STATIC
   env.cpp
   filesys.cpp
   module.cpp
   string.cpp
   time.cpp
)
