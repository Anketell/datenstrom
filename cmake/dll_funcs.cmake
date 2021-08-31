#
# Copyright (C) 2021 Lucid Systems Pty Ltd - All Rights Reserved
#

function( dll_export )
   set( OPTIONS )
   set( ONE_VALUE_ARGS TARGET )
   set( MULTI_VALUE_ARGS DATA FUNCTIONS )

   cmake_parse_arguments( ARGS "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

   if( NOT DEFINED ARGS_TARGET )
      message( FATAL_ERROR "TARGET not defined" )
   endif( NOT DEFINED ARGS_TARGET )

   if( NOT DEFINED ARGS_FUNCTIONS AND NOT DEFINED ARGS_DATA )
      message( FATAL_ERROR "Neither FUNCTIONS nor DATA are defined" )
   endif( NOT DEFINED ARGS_FUNCTIONS AND NOT DEFINED ARGS_DATA )

   set( DEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.def" )

   message( "Creating ${DEF_FILE}" )

   foreach( DATA ${ARGS_DATA} )
      set( EXPORTS "${EXPORTS}   ${DATA}   DATA\n")
   endforeach( DATA )

   foreach( FUNCTION ${ARGS_FUNCTIONS} )
      set( EXPORTS "${EXPORTS}   ${FUNCTION}\n")
   endforeach( FUNCTION )

   FILE( WRITE ${DEF_FILE} "LIBRARY ${TARGET}\nEXPORTS\n${EXPORTS}" )

   add_custom_target( "${ARGS_TARGET}_def"
      DEPENDS ${DEF_FILE}
      COMMENT "DEF file for target ${ARGS_TARGET}"
   )

#   add_dependencies( ${ARGS_TARGET} "${ARGS_TARGET}_def" )

   target_link_options( ${ARGS_TARGET} PRIVATE "/DEF:${DEF_FILE}" )

endfunction( dll_export )


function( dll_def )
   set( OPTIONS )
   set( ONE_VALUE_ARGS TARGET )
   set( MULTI_VALUE_ARGS )

   cmake_parse_arguments( ARGS "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

   if( NOT DEFINED ARGS_TARGET )
      message( FATAL_ERROR "TARGET not defined" )
   endif( NOT DEFINED ARGS_TARGET )

   set( DEF_FILE "${CMAKE_SOURCE_DIR}/win32/def/${ARGS_TARGET}.def")
   target_link_options( ${ARGS_TARGET} PRIVATE "/DEF:${DEF_FILE}" )

endfunction( dll_def )


function( module_def )
   set( OPTIONS )
   set( ONE_VALUE_ARGS TARGET )
   set( MULTI_VALUE_ARGS )

   cmake_parse_arguments( ARGS "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

   if( NOT DEFINED ARGS_TARGET )
      message( FATAL_ERROR "TARGET not defined" )
   endif( NOT DEFINED ARGS_TARGET )

   set( DEF_FILE "${CMAKE_SOURCE_DIR}/win32/def/module.def")
   target_link_options( ${ARGS_TARGET} PRIVATE "/DEF:${DEF_FILE}" )

endfunction( module_def )
