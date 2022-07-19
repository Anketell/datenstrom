#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

if ( WIN32 )

    if ( "${DS_ARCH}" STREQUAL "x64" )

        include_directories( "$ENV{PROGRAMFILES}/Firebird/Firebird_2_5/include" )
        add_library( fbclient IMPORTED STATIC GLOBAL )
        set_target_properties( fbclient
            PROPERTIES "IMPORTED_LOCATION" "$ENV{PROGRAMFILES}/Firebird/Firebird_2_5/lib/fbclient_ms.lib"
        )
    elseif ( "${DS_ARCH}" STREQUAL "x86" )

        include_directories( "$ENV{PROGRAMFILES} (x86)/Firebird/Firebird_2_5/include" )
        add_library( fbclient IMPORTED STATIC GLOBAL )
        set_target_properties( fbclient
            PROPERTIES "IMPORTED_LOCATION" "$ENV{PROGRAMFILES} (x86)/Firebird/Firebird_2_5/lib/fbclient_ms.lib"
        )
    endif()

endif ( WIN32 )
