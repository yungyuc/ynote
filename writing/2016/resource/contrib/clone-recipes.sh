#!/bin/bash

ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"
RECIPEDIR=${ROOT}/recipes
ROUTE=https://github.com/
#ROUTE=git@github.com:
PKGS="python boost pybind11 cython"

mkdir -p ${RECIPEDIR}
cd ${RECIPEDIR}
for pkgname in ${PKGS}; do
  if [ -d ${RECIPEDIR}/${pkgname}-feedstock ]; then
    echo "Directory ${RECIPEDIR}/${pkgname}-feedstock already exists"
  else
    echo "Clone feedstock to ${RECIPEDIR}/${pkgname}"
    git clone ${ROUTE}conda-forge/${pkgname}-feedstock.git
  fi
done

# vim: set et nobomb fenc=utf8 ft=sh ff=unix sw=2 ts=2:
