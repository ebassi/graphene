#!/bin/bash

builddir=_build
srcdir=$( pwd )

meson --prefix /usr --wrap-mode=nodownload "$@" ${builddir} ${srcdir} || exit $?

cd ${builddir}

ninja || exit $?
meson test || {
  res=$?
  cat meson-logs/testlog.txt
  exit $res
}

cd ..
