# Graphene - A small layer of types for graphic libraries

When creating graphic libraries you most likely end up dealing with points and
rectangles. If you're particularly unlucky, you may end up dealing with affine
matrices and 2D transformations. If you're writing a graphic library with 3D
transformations, though, you are going to hit the jackpot: 4x4 matrices,
projections, transformations, vectors, and quaternions.

Most of this stuff exists, in various forms, in many libraries; it also comes
with the rest of the libraries, which may or may not be what you want.

For this reason, I wrote the smallest possible layer needed to write a canvas
library: Graphene.

This library provides types and relative API; it does not deal with windowing
system surfaces, drawing, or event handling. You're supposed to do that
yourself.

### Dependencies

Graphene is a C library. For this reason alone, it depends on GLib, as GLib
gives us a fairly decent, portable, and maintained layer instead of
re-implementing a ton of code.

### Installation

In order to build and install Graphene you will need development tools and
the headers of the dependencies; after you've installed them, run:

    $ ./configure
    $ make
    # make install

If you have clone from Git, run:

    $ ./autogen.sh
    $ make
    # make install

### License

Graphene is released under the terms of the MIT/X11 license.

See the `LICENSE` file for more details.
