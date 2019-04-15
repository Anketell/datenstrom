#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

cd ${root}/coverage

rm -rf html

lcov --directory . --zerocounters

${root}/coverage/src/ds/test/test_ds
${root}/coverage/src/csv/test/test_ds_csv
${root}/coverage/src/sqlite/test/test_ds_sqlite
${root}/coverage/src/db/test/test_ds_db

lcov --directory . --capture --output-file /tmp/coverage.info
lcov --remove /tmp/coverage.info '*/test/*' '/usr/*' --output-file /tmp/coverage.cleaned
genhtml -o html /tmp/coverage.cleaned
rm /tmp/coverage.info /tmp/coverage.cleaned
