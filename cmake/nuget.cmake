MACRO( nuget PACKAGE VERSION )
EXEC_PROGRAM( nuget
   ARGS install ${PACKAGE} -Version ${VERSION} -ExcludeVersion -Source ${NUGET_REPO} -OutputDirectory ${NUGET_OUTPUT_DIR}
)
ENDMACRO( nuget )
