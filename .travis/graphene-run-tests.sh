#!/bin/bash

builddir=$( mktemp -d build_XXXXXX )
srcdir=$( pwd )

meson --prefix /usr --wrap-mode=nodownload "$@" ${builddir} ${srcdir} || exit $?

cd ${builddir}

ninja || exit $?
meson test || exit $?

cd ..

rm -rf ${builddir}
