if ( WIN32 )

    include_directories( "$ENV{PROGRAMFILES}/Firebird/Firebird_2_5/include" )
    add_library( fbclient IMPORTED STATIC GLOBAL )
    set_target_properties( fbclient
        PROPERTIES "IMPORTED_LOCATION" "$ENV{PROGRAMFILES}/Firebird/Firebird_2_5/lib/fbclient_ms.lib"
    )

endif ( WIN32 )