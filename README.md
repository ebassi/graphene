# Graphene
###  A thin layer of types for graphic libraries

[![Build Status](https://travis-ci.org/ebassi/graphene.svg?branch=master)](https://travis-ci.org/ebassi/graphene)

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

Graphene has little dependencies.

Graphene contains optimizations for speeding up vector operations; those
optimizations are optional, and used only if both Graphene was compiled
with support for them *and* if the system you're running on has them.
Currently, Graphene supports the following platform-specific fast paths:

 * Streaming SIMD Extensions (SSE) 2
 * ARM NEON
 * GCC vector extensions

In the remote case in which none of these optimizations are available,
Graphene will fall back to a naive scalar implementation.

Graphene can, optionally, provide types for integrating with GObject
properties and signals, as well as introspection information for its
use with other languages through introspection-based bindings.

### Installation

In order to build and install Graphene you will need development tools and
the headers of the dependencies.

First of all, clone the Git repository:

    $ git clone git://github.com/ebassi/graphene
    $ cd graphene

Then run:

    $ ./autogen.sh
    $ make
    $ make check
    # make install

It is possible, when building Graphene, to disable specific optimizations by
passing options to the `configure` script:

 * `--disable-sse2` - will disable the SSE2 fast paths
 * `--disable-arm-neon` - will disable the ARM NEON fast paths
 * `--disable-gcc-vector` - will disable the GCC vector intrinsics

If you don't plan on generating introspection data, use `--disable-introspection`
when configuring Graphene; similarly, if you don't plan on using GObject with
Graphene, use `--disable-gobject-types`. Disabling GObject types will also
automatically disable generating introspection data.

You can also disable building the test suite and the benchmark suite, using
the `--disable-tests` and `--disable-benchmarks` configuration switches
respectively.

See the output of `configure --help` for more information on the available
configuration switches and environment variables.

### Contributing

If you found a bug (which I'm sure there will be plenty), or if you want
to add your own pet feature, then follow these steps:

 1. Fork the [ebassi/graphene](https://github.com/ebassi/graphene) repo
 2. Fix bugs or add new features and push them to your clone
 3. Open [a new issue](https://github.com/ebassi/graphene/issues/new)
 4. Open [a pull request](https://github.com/ebassi/graphene/pulls)
 5. Wait for me to give feedback on the pull request
 6. Celebrate when your code gets merged

That's pretty much it.

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

Graphene also provides its low-level SIMD vector and matrix types, which are
used to implement the API above.

All types can be placed on the stack, but provide allocation/free functions
for working on the heap as well. The contents of all structure types, except
for points and rectangles, should be considered private, and should never be
accessed directly.

The full API reference for the for Graphene is available online here:
https://people.gnome.org/~ebassi/docs/graphene/

### License

Graphene is released under the terms of the MIT/X11 license.

See the `LICENSE` file for more details.
