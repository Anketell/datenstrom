#!/usr/bin/bash

pushd $(dirname $0)/.. >/dev/null
root=${PWD}

rm -rf ${root}/release/rpmbuild

${root}/scripts/make_image.sh

mkdir -p ${root}/release/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

export VERSION=$(sed -e "s/^\(.*\)-.*$/\1/" < ${root}/scripts/VERSION)
export RELEASE=$(sed -e "s/^.*-\(.*\)$/\1/" < ${root}/scripts/VERSION)

mv ${root}/release/image \
   ${root}/release/rpmbuild/BUILD/datenstrom-devel-${VERSION}

bash -c "echo \"$(cat ${root}/scripts/datenstrom-devel.spec.tmpl)"\" > \
         ${root}/release/rpmbuild/SPECS/datenstrom-devel.spec

cd release/rpmbuild/BUILD
 tar czf ${root}/release/rpmbuild/SOURCES/datenstrom-devel.tar.gz \
         datenstrom-devel-${VERSION}

cd ${root}

rpmbuild --define "_topdir ${root}/release/rpmbuild/" -vvv -bb \
         ${root}/release/rpmbuild/SPECS/datenstrom-devel.spec

popd >/dev/null

