#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
   set( DS_ARCH "x64" )
else()
   set( DS_ARCH "x86" )
endif()

