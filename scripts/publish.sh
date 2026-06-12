#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}

env=${root}/scripts/.env

if [ -f ${env} ]; then
   set -o allexport
   . ${env}
   set +o allexport
fi

export PACKAGE=${root}/release/rpmbuild/RPMS/x86_64/*.rpm
export BASENAME=$(basename ${PACKAGE})

# curl -H "Content-Disposition: attachment; filename=\"${BASENAME}\"" -T ${PACKAGE} ${DS_REPO}
#
curl -T ${PACKAGE} ${DS_REPO}/rpms/${BASENAME}

popd >/dev/null
