function( dll_export )
   set( OPTIONS )
   set( ONE_VALUE_ARGS TARGET )
   set( MULTI_VALUE_ARGS EXPORTS )

   cmake_parse_arguments( ARGS "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

   if( NOT DEFINED ARGS_TARGET )
      message( FATAL_ERROR "TARGET not defined" )
   endif( NOT DEFINED ARGS_TARGET )

   if( NOT DEFINED ARGS_EXPORTS )
   message( FATAL_ERROR "EXPORTS not defined" )
   endif( NOT DEFINED ARGS_EXPORTS )

   set( DEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.def" )

   message( "Creating ${DEF_FILE}" )

   foreach( EXPORT ${ARGS_EXPORTS} )
      set( EXPORTS "${EXPORTS}   ${EXPORT}\n")
   endforeach( EXPORT )

   FILE( WRITE ${DEF_FILE} "LIBRAY ${TARGET}\nEXPORTS\n${EXPORTS}" )

   add_custom_target( "${ARGS_TARGET}_def"
      DEPENDS ${DEF_FILE}
      COMMENT "DEF file for target ${ARGS_TARGET}"
   )

   add_dependencies( ${ARGS_TARGET} "${ARGS_TARGET}_def" )

   target_link_options( ${TARGET} PRIVATE "/DEF:${DEF_FILE}" )

endfunction( dll_export )
