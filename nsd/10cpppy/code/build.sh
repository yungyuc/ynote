#!/bin/bash
#
# Copyright (C) 2018-2021 Yung-Yu Chen <yyc@solvcon.net>.
#
# Build and install nsd dependencies.

libst() {

  mysudo="$1"
  githuborg=yungyuc
  pkgname=turgon
  pkgbranch=master
  pkgfull=turgon-master
  pkgrepo=https://github.com/$githuborg/$pkgname.git
  repotar=https://github.com/$githuborg/$pkgname/archive/$pkgbranch.tar.gz

  if [ ! -d $pkgfull ] ; then
    git clone -q -b $pkgbranch $pkgrepo $pkgfull
  else
    pushd $pkgfull
    if [ -z "$NOPULL" ] ; then
      git co $pkgbranch
      git pull origin $pkgbranch
    fi
    popd
  fi

  pushd $pkgfull/spacetime
  make MODMESH_ROOT=../../modmesh-master VERBOSE=1
  popd

}

modmesh() {

  mysudo="$1"
  githuborg=solvcon
  pkgname=modmesh
  pkgbranch=master
  pkgfull=modmesh-master
  pkgrepo=https://github.com/$githuborg/$pkgname.git
  repotar=https://github.com/$githuborg/$pkgname/archive/$pkgbranch.tar.gz

  if [ ! -d $pkgfull ] ; then
    git clone -q -b $pkgbranch $pkgrepo $pkgfull
  else
    pushd $pkgfull
    if [ -z "$NOPULL" ] ; then
      git co $pkgbranch
      git pull origin $pkgbranch
    fi
    popd
  fi

  pushd $pkgfull
  make
  popd

}

# Run function.
eval "$1"
