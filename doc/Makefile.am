DOC_MODULE = graphene
DOC_MAIN_SGML_FILE = graphene-docs.xml
DOC_SOURCE_DIR = $(top_srcdir)/src

SCAN_OPTIONS = --rebuild-types
SCANGOBJ_OPTIONS =
MKDB_OPTIONS = --output-format=xml --name-space=graphene
FIXXREF_OPTIONS =

HFILE_GLOB = $(top_srcdir)/src/*.h $(top_builddir)/src/*.h
CFILE_GLOB = $(top_srcdir)/src/*.c

IGNORE_HFILES = \
	$(top_builddir)/src/config.h \
	$(top_builddir)/src/graphene-config.h \
	$(top_srcdir)/src/graphene.h \
	$(top_srcdir)/src/graphene-private.h \
	$(top_srcdir)/src/graphene-alloc-private.h \
	$(top_srcdir)/src/graphene-line-segment.h \
	$(top_srcdir)/src/graphene-macros.h \
	$(top_srcdir)/src/graphene-version-macros.h \
	$(top_srcdir)/src/graphene-vectors-private.h \
	$(top_srcdir)/src/bench/graphene-bench-utils.h \
	$(top_srcdir)/src/tests/graphene-test-compat.h

EXTRA_HFILES =

HTML_IMAGES =

content_files =
expand_content_files = 

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src $(GRAPHENE_CFLAGS)
GTKDOC_LIBS = $(top_builddir)/src/libgraphene-1.0.la $(GRAPHENE_LIBS)

include $(top_srcdir)/gtk-doc.make

API_VERSION = $(GRAPHENE_MAJOR_VERSION).$(GRAPHENE_MINOR_VERSION)
SSH_HOST = master.gnome.org
SSH_BASEPATH = ~ebassi/public_html/docs/graphene
SSH_FULLPATH = $(SSH_BASEPATH)/$(API_VERSION)

publish:
	scp -r ./html/* $(SSH_HOST):$(SSH_FULLPATH)

.PHONY: publish
