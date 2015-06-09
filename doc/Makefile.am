DOC_MODULE = graphene
DOC_MAIN_SGML_FILE = graphene-docs.xml
DOC_SOURCE_DIR = $(top_srcdir)/src $(top_builddir)/src

SCAN_OPTIONS = --rebuild-types
SCANGOBJ_OPTIONS =
MKDB_OPTIONS = --output-format=xml --name-space=graphene
FIXXREF_OPTIONS =

HFILE_GLOB = $(top_srcdir)/src/*.h $(top_builddir)/src/*.h
CFILE_GLOB = $(top_srcdir)/src/*.c

IGNORE_HFILES = \
	config.h \
	graphene-config.h \
	graphene.h \
	graphene-private.h \
	graphene-alloc-private.h \
	graphene-line-segment.h \
	graphene-macros.h \
	graphene-version-macros.h \
	graphene-vectors-private.h \
	graphene-bench-utils.h \
	graphene-test-compat.h

EXTRA_HFILES =

HTML_IMAGES =

content_files =
expand_content_files = 

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src -I$(top_builddir)/src $(GRAPHENE_CFLAGS)
GTKDOC_LIBS = $(top_builddir)/src/libgraphene-1.0.la $(GRAPHENE_LIBS)

include $(top_srcdir)/gtk-doc.make

if ENABLE_GTK_DOC
TESTS_ENVIRONMENT = \
	DOC_MODULE=$(DOC_MODULE) DOC_MAIN_SGML_FILE=$(DOC_MAIN_SGML_FILE) \
	SRCDIR=$(abs_srcdir) BUILDDIR=$(abs_builddir)
TESTS = $(GTKDOC_CHECK)
endif
