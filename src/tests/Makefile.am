include $(top_srcdir)/build/autotools/glib-tap.mk

AM_CFLAGS = -g $(GRAPHENE_CFLAGS) $(GLIB_CFLAGS)
AM_CPPFLAGS = \
	$(GRAPHENE_DEBUG_CFLAGS) \
	-DG_LOG_DOMAIN=\"Graphene\" \
	-DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_30 \
	-DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_32 \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src
LDADD = $(top_builddir)/src/libgraphene-1.0.la $(GRAPHENE_LIBS) $(GLIB_LIBS)

dist_uninstalled_test_data = graphene-test-compat.h

test_programs = \
	point \
	point3d \
	size \
	rect \
	quad \
	vec2 \
	vec3 \
	vec4 \
	matrix
