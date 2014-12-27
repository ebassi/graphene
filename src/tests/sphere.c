#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (sphere_init)
{
  graphene_point3d_t check;
  graphene_sphere_t *s;

  s = graphene_sphere_init (graphene_sphere_alloc (), &zero3, 1.f);
  g_assert_nonnull (s);
  g_assert_false (graphene_sphere_is_empty (s));

  graphene_sphere_get_center (s, &check);
  g_assert_true (graphene_point3d_equal (&zero3, &check));
  g_assert_cmpfloat (graphene_sphere_get_radius (s), ==, 1.f);

  graphene_sphere_init (s, NULL, 1.f);
  graphene_sphere_get_center (s, &check);
  g_assert_true (graphene_point3d_equal (&zero3, &check));
  g_assert_cmpfloat (graphene_sphere_get_radius (s), ==, 1.f);

  graphene_sphere_free (s);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (sphere_contains_point)
{
  graphene_point3d_t check;
  graphene_sphere_t *s;

  s = graphene_sphere_init (graphene_sphere_alloc (), &zero3, 1.f);
  g_assert_nonnull (s);

  g_assert_true (graphene_sphere_contains_point (s, &zero3));

  graphene_point3d_init (&check, 1.f, 0.f, 0.f);
  g_assert_true (graphene_sphere_contains_point (s, &check));

  graphene_point3d_init (&check, 0.f, -1.f, 0.f);
  g_assert_true (graphene_sphere_contains_point (s, &check));

  g_assert_false (graphene_sphere_contains_point (s, &one3));

  graphene_sphere_free (s);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (sphere_bounding_box)
{
  graphene_sphere_t s;
  graphene_box_t b, check;
  graphene_point3d_t tmp;

  graphene_box_init (&check, &zero3, graphene_point3d_init (&tmp, 2.f, 2.f, 2.f));

  graphene_sphere_init (&s, &one3, 1.f);
  graphene_sphere_get_bounding_box (&s, &b);
  g_assert_true (graphene_box_equal (&b, &check));

  graphene_sphere_init (&s, &zero3, 0.f);
  graphene_sphere_get_bounding_box (&s, &b);
  g_assert_true (graphene_box_equal (&b, graphene_box_zero ()));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (sphere_distance)
{
  graphene_sphere_t s;

  graphene_sphere_init (&s, &one3, 1.f);
  g_assert_cmpfloat (graphene_sphere_distance (&s, &zero3) - 0.7320, <, 0.001);
  g_assert_cmpfloat (graphene_sphere_distance (&s, &one3), ==, -1.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (sphere_translate)
{
  graphene_sphere_t s;
  graphene_point3d_t p;

  graphene_sphere_init (&s, &one3, 1.f);

  graphene_point3d_init (&p, -1.f, -1.f, -1.f);
  graphene_sphere_translate (&s, &p, &s);
  graphene_sphere_get_center (&s, &p);
  g_assert_true (graphene_point3d_equal (&p, &zero3));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (sphere_empty)
{
  graphene_sphere_t s;

  graphene_sphere_init (&s, NULL, 0.f);
  g_assert_true (graphene_sphere_is_empty (&s));

  graphene_sphere_init (&s, &one3, 1.f);
  g_assert_false (graphene_sphere_is_empty (&s));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/sphere/init", sphere_init)
  GRAPHENE_TEST_UNIT ("/sphere/contains-point", sphere_contains_point)
  GRAPHENE_TEST_UNIT ("/sphere/bounding-box", sphere_bounding_box)
  GRAPHENE_TEST_UNIT ("/sphere/distance", sphere_distance)
  GRAPHENE_TEST_UNIT ("/sphere/translate", sphere_translate)
  GRAPHENE_TEST_UNIT ("/sphere/empty", sphere_empty)
)
