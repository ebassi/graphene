include $(top_srcdir)/build/autotools/glib-tap.mk

AM_CFLAGS = -g $(GRAPHENE_CFLAGS) $(MAINTAINER_CFLAGS)
AM_CPPFLAGS = \
	$(GRAPHENE_DEBUG_CFLAGS) \
	-DG_LOG_DOMAIN=\"Graphene\" \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src
LDADD = $(top_builddir)/src/libgraphene-1.0.la $(GRAPHENE_LIBS)

test_programs = \
	point \
	point3d \
	size \
	rect \
	quad \
	vec2 \
	vec3 \
	matrix
