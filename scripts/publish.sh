#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}

. ${root}/scripts/import_env.sh

export PACKAGE=${root}/release/rpmbuild/RPMS/x86_64/*.rpm
export BASENAME=$(basename ${PACKAGE})

# curl -H "Content-Disposition: attachment; filename=\"${BASENAME}\"" -T ${PACKAGE} ${DS_REPO}
#

curl -T ${PACKAGE} ${DS_REPO}/rpms/${BASENAME}

popd >/dev/null
