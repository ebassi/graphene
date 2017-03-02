#!/bin/sh
test -n "$srcdir" || srcdir=$(dirname "$0")
test -n "$srcdir" || srcdir=.

cd $srcdir

VERSION=$(git describe --abbrev=0)
NAME="graphene-$VERSION"

rm -f "${NAME}.tar"

echo "Creating git tree archive…"
git archive --prefix="${NAME}/" --format=tar HEAD > ${NAME}.tar

echo "Compressing archive…"
xz -f "${NAME}.tar"
