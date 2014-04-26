# preamble
NULL =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES =
lib_LTLIBRARIES =

# always build this directory before building the tests
SUBDIRS = . tests

# source
source_h = \
	graphene-macros.h \
	graphene-point.h \
	graphene-vec2.h \
	graphene-vec3.h \
	graphene-vec4.h \
	graphene-types.h \
	$(NULL)
source_c = \
	graphene-point.c \
	graphene-size.c \
	graphene-vectors.c \
	$(NULL)

source_h_priv = \
	graphene-simd4f.h \
	graphene-simd4f-common.h \
	graphene-simd4f-gcc.h \
	graphene-simd4f-neon.h \
	graphene-simd4f-scalar.h \
	graphene-simd4f-sse.h \
	graphene-vectors-private.h \
	$(NULL)
source_c_priv =

# main library
libgraphene_1_0_la_CPPFLAGS = \
	-DGRAPHENE_COMPILATION=1 \
	-DG_LOG_DOMAIN=\"Graphene\" \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src \
	$(GRAPHENE_DEBUG_CFLAGS)
libgraphene_1_0_la_CFLAGS = $(GRAPHENE_CFLAGS) $(MAINTAINER_CFLAGS)
libgraphene_1_0_la_LIBADD = $(GRAPHENE_LIBS)
libgraphene_1_0_la_LDFLAGS = $(GRAPHENE_LDFLAGS) -export-dynamic
libgraphene_1_0_la_SOURCES = $(source_h) $(source_h_priv) $(source_c) $(source_c_priv) $(BUILT_SOURCES)

lib_LTLIBRARIES += libgraphene-1.0.la

# headers
grapheneincludedir = $(includedir)/graphene-1.0
grapheneinclude_DATA = $(source_h) graphene.h

# pkg-config files
graphene-1.0.pc: graphene.pc
	$(AM_V_GEN)cp -f $< $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = graphene-1.0.pc
CLEANFILES += graphene-1.0.pc
EXTRA_DIST += graphene.pc.in
