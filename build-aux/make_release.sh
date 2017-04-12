#!/bin/sh
set -x

test -n "$srcdir" || srcdir=$1
test -n "$srcdir" || srcdir=.

cd "$srcdir"

PROJECT=graphene
VERSION=$(git describe --abbrev=0)
NAME="${PROJECT}-${VERSION}"

rm -f "${NAME}.tar"

echo "Creating git tree archive…"
git archive --prefix="${NAME}/" --format=tar HEAD > ${NAME}.tar

rm -f "${NAME}.tar.xz"

echo "Compressing archive…"
xz -f "${NAME}.tar"

set +x
