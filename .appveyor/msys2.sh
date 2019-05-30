#!/bin/bash

set -e

if [[ "$MSYSTEM" == "MINGW32" ]]; then
    export MSYS2_ARCH="i686"
else
    export MSYS2_ARCH="x86_64"
fi

# Update everything
pacman --noconfirm -Suy

# Install the required packages
pacman --noconfirm -S --needed \
    base-devel \
    mingw-w64-$MSYS2_ARCH-gobject-introspection
    mingw-w64-$MSYS2_ARCH-meson \
    mingw-w64-$MSYS2_ARCH-ninja \
    mingw-w64-$MSYS2_ARCH-pkg-config \
    mingw-w64-$MSYS2_ARCH-python3 \
    mingw-w64-$MSYS2_ARCH-python3-pip \
    mingw-w64-$MSYS2_ARCH-toolchain \

pip3 install --upgrade --user meson==0.50.1
export PATH="$HOME/.local/bin:$PATH"

# Build
meson --buildtype=debug _build
cd _build
ninja

# Test
meson test
