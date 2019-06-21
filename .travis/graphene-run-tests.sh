#!/bin/bash

set -x

builddir=_build
srcdir=$( pwd )

export CFLAGS='-coverage -ftest-coverage -fprofile-arcs'

meson --prefix /usr --wrap-mode=nodownload "$@" ${builddir} ${srcdir} || exit $?

ninja -C $builddir || exit $?
meson test -C $builddir || {
  res=$?
  cat meson-logs/testlog.txt
  exit $res
}

cpp-coveralls \
        -r . -b $builddir \
        -i src -i ../src \
        -e src/tests -e ../src/tests \
        -e subprojects -e ../subprojects \
        --gcov-options='\-lp'
