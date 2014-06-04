# preamble
NULL =
AM_CFLAGS =
LDADD =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST =
DISTCLEANFILES =
check_PROGRAMS =
noinst_PROGRAMS =
noinst_LTLIBRARIES =
TESTS =

# common library for benchmark utilities
libgraphene_bench_la_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src -DG_LOG_DOMAIN=\"Graphene-Bench\"
libgraphene_bench_la_CFLAGS = $(GRAPHENE_CFLAGS) $(MAINTAINER_CFLAGS)
libgraphene_bench_la_LIBADD = $(GRAPHENE_LIBS)
libgraphene_bench_la_SOURCES = graphene-bench-utils.h graphene-bench-utils.c
noinst_LTLIBRARIES += libgraphene-bench.la

# common flags for benchmarks
common_cflags = $(GRAPHENE_CFLAGS) -I$(top_srcdir)/src -I$(top_builddir)/src -DGRAPHENE_COMPILATION=1
common_ldadd = $(GRAPHENE_LIBS) libgraphene-bench.la

AM_CFLAGS += $(common_cflags)
LDADD += $(common_ldadd)
noinst_PROGRAMS += matrix
