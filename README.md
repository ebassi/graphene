# Graphene
###  A thin layer of types for graphic libraries

When creating graphic libraries you most likely end up dealing with points
and rectangles. If you're particularly unlucky, you may end up dealing
with affine matrices and 2D transformations. If you're writing a graphic
library with 3D transformations, though, you are going to hit the jackpot:
4x4 matrices, projections, transformations, vectors, and quaternions.

Most of this stuff exists, in various forms, in other libraries, but it
has the major drawback of coming along with the rest of those libraries,
which may or may not be what you want. Those libraries are also available
in various languages, as long as those languages are C++; again, it may or
may not be something you want.

For this reason, I decided to write the thinnest, smallest possible layer
needed to write a canvas library; given its relative size, and the
propensity for graphics libraries to have a pun in their name, I decided
to call it Graphene.

This library provides types and their relative API; it does not deal with
windowing system surfaces, drawing, scene graphs, or input. You're
supposed to do that yourself, in your own canvas implementation, which is
the whole point of writing the library in the first place.

### Dependencies

Graphene is a C library. For this reason alone, it depends on GLib, as
GLib gives us a fairly decent, portable, and maintained layer to depend
on, instead of requiring me to re-implement a ton of code.

Graphene does not use the GObject type system, as of yet, but it may grow
an optional dependency on it through a separate shared object, especially
if in the future I'm going to generate introspection data for the benefit
of GObject-based language bindings.

Graphene contains optimizations for speeding up vector operations; those
optimizations are optional, and used only if both Graphene was compiled
with support for them *and* if the system you're running on has them.
Currently, Graphene supports the following platform-specific fast paths:

 * Streaming SIMD Extensions (SSE) 2
 * ARM NEON
 * GCC vector extensions

In the remote case in which none of these optimizations are available,
Graphene will fall back to a naive scalar implementation.

### Installation

In order to build and install Graphene you will need development tools and
the headers of the dependencies; after you've installed them, run:

    $ ./configure
    $ make
    # make install

If you have a clone from Git, run:

    $ ./autogen.sh
    $ make
    # make install

It is possible, when building Graphene, to disable specific optimizations by
passing options to the `configure` script:

 * `--disable-sse2` - will disable the SSE2 fast paths
 * `--disable-arm-neon` - will disable the ARM NEON fast paths
 * `--disable-gcc-vector` - will disable the GCC vector intrinsics

See the output of `configure --help` for more information.

## Documentation

### Available types

Graphene provides common types needed to handle 3D transformations:

 * 2D points
 * 3D points
 * rectangles
 * quads
 * quaternions
 * vectors (2, 3, or 4-sized)
 * matrices

All types can be placed on the stack, but provide allocation/free functions
for working on the heap as well. The contents of all structure types, except
for points and rectangles, should be considered private, and should never be
accessed directly.

### License

Graphene is released under the terms of the MIT/X11 license.

See the `LICENSE` file for more details.
