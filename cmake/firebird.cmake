#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

if ( WIN32 )

    if ( "${DS_ARCH}" STREQUAL "x64" )

        file( GLOB FIREBIRD_DIR "$ENV{PROGRAMFILES}/Firebird/Firebird*" )

    elseif ( "${DS_ARCH}" STREQUAL "x86" )

        file( GLOB FIREBIRD_DIR "$ENV{PROGRAMFILES} (x86)/Firebird/Firebird*" )

    endif()

    MESSAGE( STATUS "Firebird: ${FIREBIRD_DIR}")

    include_directories( "${FIREBIRD_DIR}/include" )
    add_library( fbclient IMPORTED STATIC GLOBAL )
    set_target_properties( fbclient
        PROPERTIES "IMPORTED_LOCATION" "${FIREBIRD_DIR}/lib/fbclient_ms.lib"
    )

endif ( WIN32 )
