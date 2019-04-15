FIND_PROGRAM(LCOV_PATH lcov)
FIND_PROGRAM(GENHTML_PATH genhtml)

set(CMAKE_CXX_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )

set(CMAKE_C_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )

add_custom_target( check
    COMMAND ${LCOV_PATH} --directory . --zerocounters
    COMMAND ctest
    COMMAND ${LCOV_PATH} --directory . --capture --output-file /tmp/coverage.info
    COMMAND ${LCOV_PATH} --remove /tmp/coverage.info 'test*/*' '/usr/*' --output-file /tmp/coverage.cleaned
    COMMAND ${GENHTML_PATH} -o html /tmp/coverage.cleaned
)
