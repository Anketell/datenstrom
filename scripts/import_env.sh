#!/usr/bin/bash
#------------------------------------------------------------------------------

pushd $(dirname $0)/.. >/dev/null
root=${PWD}
popd >/dev/null

env=${root}/scripts/.env

if [ -f ${env} ]; then
   . ${env}
else
   echo
   echo Error: ${env} undefined
   echo
   echo Example:
   echo
   cat ${root}/scripts/env.sample
   exit 1
fi

#------------------------------------------------------------------------------
