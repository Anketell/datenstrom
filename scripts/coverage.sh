#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

cd ${root}/coverage

rm -rf html

lcov --directory . --zerocounters

ctest

lcov --ignore-errors mismatch --directory . --capture --output-file /tmp/coverage.info
lcov --remove /tmp/coverage.info '*/test*/*' '*/third-party/*' '/usr/*' --output-file /tmp/coverage.cleaned
genhtml -o html /tmp/coverage.cleaned
rm /tmp/coverage.info /tmp/coverage.cleaned
