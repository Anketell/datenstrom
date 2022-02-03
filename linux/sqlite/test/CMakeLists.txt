#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

set( TARGET sqlite_native_test_data )

set( SOURCE
   sqlite_native_test_data.cpp
)

add_library( ${TARGET} STATIC
   ${SOURCE}
)
