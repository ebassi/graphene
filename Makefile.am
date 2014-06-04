ACLOCAL_AMFLAGS = -I build/autotools

EXTRA_DIST = \
	build/autotools/glibtests.m4 \
	build/autotools/glib-tap.mk \
	build/autotools/tap-driver.sh \
	build/autotools/tap-test

SUBDIRS = src doc

DISTCHECK_CONFIGURE_FLAGS = --enable-maintainer-flags --enable-gtk-doc

if ENABLE_GCOV
# use recursive makes in order to ignore errors during check/perf
lcov:
	-$(MAKE) $(AM_MAKEFLAGS) check
	$(MAKE) $(AM_MAKEFLAGS) genlcov

# we have to massage the lcov.info file slightly to hide the effect of libtool
# placing the objects files in the .libs/ directory separate from the *.c
genlcov:
	$(LTP) --directory $(top_builddir) --capture --output-file graphene-lcov.info --test-name GRAPHENE_TEST --no-checksum
	$(SED) -e 's#.libs/##' < graphene-lcov.info > graphene-lcov.info.tmp
	LANG=C $(LTP_GENHTML) --prefix $(top_builddir) --output-directory graphene-lcov --title "Graphene Code Coverage" --show-details graphene-lcov.info.tmp
	rm -f graphene-lcov.info.tmp

lcov-clean:
	-$(LTP) --directory $(top_builddir) -z
	-$(RM) -rf graphene-lcov.info graphene-lcov
else
lcov genlcov lcov-clean:
	@echo You need to configure Graphene with --enable-code-coverage
endif

.PHONY: lcov genlcov lcov-clean
