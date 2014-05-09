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
common_cflags = $(GRAPHENE_CFLAGS)
common_ldadd = $(GRAPHENE_LIBS) $(top_builddir)/src/libgraphene-1.0.la libgraphene-bench.la
common_cppflags = -DGRAPHENE_BENCHMARK=1
common_scalar_cppflags = $(common_cppflags) -DGRAPHENE_HAS_SCALAR=1 -DGRAPHENE_BENCH=\"scalar\"
common_gcc_cppflags = $(common_cppflags) -DGRAPHENE_HAS_GCC=1 -DGRAPHENE_BENCH=\"gcc\"
common_sse_cppflags = $(common_cppflags) -DGRAPHENE_HAS_SSE=1 -DGRAPHENE_BENCH=\"sse\"

# list all tests
base_tests = vec2 vec4 matrix

if BUILD_WITH_GCC
gcc_tests = $(addprefix gcc-,$(base_tests))
endif

if BUILD_WITH_SSE2
sse_tests = $(addprefix sse-,$(base_tests))
endif

scalar_tests = $(addprefix scalar-,$(base_tests))

all_tests = $(scalar_tests) $(gcc_tests) $(sse_tests)

benchmark: $(all_tests)
	@for i in $(all_tests); do \
	  echo "Running $$i ..." ; \
	  ./$$i -d 1; \
	done

benchmark-csv: $(all_tests)
	@echo "path,implementation,min,max,avg" > benchmark-result.csv ; \
	for i in $(all_tests); do \
	  echo "Running $$i ..." ; \
	  ./$$i -f csv >> benchmark-result.csv; \
	done ; \
	cat benchmark-result.csv

.PHONY: benchmark benchmark-csv

AM_CFLAGS += $(common_cflags)
LDADD += $(common_ldadd)

scalar_vec2_CPPFLAGS = $(common_scalar_cppflags)
scalar_vec2_SOURCES = vec2.c
noinst_PROGRAMS += scalar-vec2

scalar_vec4_CPPFLAGS = $(common_scalar_cppflags)
scalar_vec4_SOURCES = vec4.c
noinst_PROGRAMS += scalar-vec4

scalar_matrix_CPPFLAGS = $(common_scalar_cppflags)
scalar_matrix_SOURCES = matrix.c
noinst_PROGRAMS += scalar-matrix

if BUILD_WITH_GCC
gcc_vec2_CPPFLAGS = $(common_gcc_cppflags)
gcc_vec2_SOURCES = vec2.c
noinst_PROGRAMS += gcc-vec2

gcc_vec4_CPPFLAGS = $(common_gcc_cppflags)
gcc_vec4_SOURCES = vec4.c
noinst_PROGRAMS += gcc-vec4

gcc_matrix_CPPFLAGS = $(common_gcc_cppflags)
gcc_matrix_SOURCES = matrix.c
noinst_PROGRAMS += gcc-matrix
endif

if BUILD_WITH_SSE2
sse_vec2_CPPFLAGS = $(common_sse_cppflags)
sse_vec2_SOURCES = vec2.c
noinst_PROGRAMS += sse-vec2

sse_vec4_CPPFLAGS = $(common_sse_cppflags)
sse_vec4_SOURCES = vec4.c
noinst_PROGRAMS += sse-vec4

sse_matrix_CPPFLAGS = $(common_sse_cppflags)
sse_matrix_SOURCES = matrix.c
noinst_PROGRAMS += sse-matrix
endif
