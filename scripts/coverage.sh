#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

cd ${root}/coverage

rm -rf html

lcov --directory . --zerocounters

DS_MODULE_PATH=/home/matth/devel/datenstrom/coverage/src/sqlite:\
/home/matth/devel/datenstrom/coverage/src/mysql:\
/home/matth/devel/datenstrom/coverage/src/firebird ctest

lcov --directory . --capture --output-file /tmp/coverage.info
lcov --remove /tmp/coverage.info '*/test*/*' '*/third-party/*' '/usr/*' --output-file /tmp/coverage.cleaned
genhtml -o html /tmp/coverage.cleaned
rm /tmp/coverage.info /tmp/coverage.cleaned
