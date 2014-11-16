#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (plane_init_base)
{
  graphene_plane_t *p;
  graphene_vec3_t n;

  p = graphene_plane_init (graphene_plane_alloc (), NULL, 0.f);
  graphene_plane_get_normal (p, &n);
  g_assert_true (graphene_vec3_equal (&n, graphene_vec3_x_axis ()));
  g_assert_cmpfloat (graphene_plane_get_constant (p), ==, 0.f);

  graphene_plane_init (p, graphene_vec3_one (), 0.f);
  graphene_plane_get_normal (p, &n);
  g_assert_true (graphene_vec3_equal (&n, graphene_vec3_one ()));
  g_assert_cmpfloat (graphene_plane_get_constant (p), ==, 0.f);

  graphene_plane_init (p, graphene_vec3_one (), 1.f);
  graphene_plane_get_normal (p, &n);
  g_assert_true (graphene_vec3_equal (&n, graphene_vec3_one ()));
  g_assert_cmpfloat (graphene_plane_get_constant (p), ==, 1.f);

  graphene_plane_free (p);
}
GRAPHENE_TEST_UNIT_END


GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/plane/init/base", plane_init_base)
)
