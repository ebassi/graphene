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

GRAPHENE_TEST_UNIT_BEGIN (plane_init_normal_point)
{
  graphene_vec3_t normal, check;
  graphene_plane_t *p;

  graphene_vec3_normalize (graphene_vec3_one (), &normal);

  p = graphene_plane_init_from_point (graphene_plane_alloc (), &normal, &zero3);

  graphene_plane_get_normal (p, &check);
  g_assert_true (graphene_vec3_equal (&check, &normal));
  g_assert_cmpfloat (graphene_plane_get_constant (p), ==, 0.f);

  graphene_plane_free (p);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (plane_normalize)
{
  graphene_plane_t p;
  graphene_vec3_t tmp;

  graphene_plane_init (&p, graphene_vec3_init (&tmp, 2.f, 0.f, 0.f), 2.f);

  graphene_plane_normalize (&p, &p);
  graphene_plane_get_normal (&p, &tmp);
  graphene_assert_fuzzy_vec3_equal (&tmp, graphene_vec3_x_axis (), 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec3_length (&tmp), 1.f, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_plane_get_constant (&p), 1.f, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (plane_distance_positive)
{
  graphene_plane_t p;
  graphene_vec3_t tmp;
  graphene_point3d_t point;

  graphene_plane_init (&p, graphene_vec3_init (&tmp, 2.f, 0.f, 0.f), -2.f);
  graphene_plane_normalize (&p, &p);

  graphene_point3d_init (&point, 4.f, 0.f, 0.f);
  graphene_assert_fuzzy_equals (graphene_plane_distance (&p, &point), 3.f, 0.0001);

  graphene_point3d_init (&point, 1.f, 0.f, 0.f);
  graphene_assert_fuzzy_equals (graphene_plane_distance (&p, &point), 0.f, 0.0001);

  graphene_plane_negate (&p, &p);

  graphene_point3d_init (&point, 4.f, 0.f, 0.f);
  graphene_assert_fuzzy_equals (graphene_plane_distance (&p, &point), -3.f, 0.0001);

  graphene_point3d_init (&point, 1.f, 0.f, 0.f);
  graphene_assert_fuzzy_equals (graphene_plane_distance (&p, &point), 0.f, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (plane_distance_negative)
{
  graphene_plane_t p;
  graphene_vec3_t tmp;
  graphene_point3d_t point;

  graphene_plane_init (&p, graphene_vec3_init (&tmp, 2.f, 0.f, 0.f), -2.f);
  graphene_plane_normalize (&p, &p);

  graphene_point3d_init (&point, 4.f, 0.f, 0.f);
  graphene_assert_fuzzy_equals (graphene_plane_distance (&p, &point), 3.f, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/plane/init/base", plane_init_base)
  GRAPHENE_TEST_UNIT ("/plane/init/normal-point", plane_init_normal_point)
  GRAPHENE_TEST_UNIT ("/plane/normalize", plane_normalize)
  GRAPHENE_TEST_UNIT ("/plane/distance/positive", plane_distance_positive)
  GRAPHENE_TEST_UNIT ("/plane/distance/negative", plane_distance_negative)
)
