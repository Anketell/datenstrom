set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library(test_model SHARED
   object.cpp
   object_serialise.cpp
)
