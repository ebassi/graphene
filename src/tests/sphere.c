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

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/sphere/init", sphere_init)
  GRAPHENE_TEST_UNIT ("/sphere/contains-point", sphere_contains_point)
)
