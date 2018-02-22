#!/bin/bash

mkdir _build

meson --prefix /usr "$@" _build . || exit $?
ninja -C _build || exit $?
ninja -C _build test || exit $?

rm -rf _build
