#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
sphere_init (void)
{
  graphene_point3d_t center = GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f);
  graphene_point3d_t check;
  graphene_sphere_t *s;

  s = graphene_sphere_init (graphene_sphere_alloc (), &center, 1.f);
  g_assert_nonnull (s);
  g_assert_false (graphene_sphere_is_empty (s));

  graphene_sphere_get_center (s, &check);
  g_assert_true (graphene_point3d_equal (&center, &check));
  g_assert_cmpfloat (graphene_sphere_get_radius (s), ==, 1.f);

  graphene_sphere_init (s, NULL, 1.f);
  graphene_sphere_get_center (s, &check);
  g_assert_true (graphene_point3d_equal (&check, &check));
  g_assert_cmpfloat (graphene_sphere_get_radius (s), ==, 1.f);

  graphene_sphere_free (s);
}

static void
sphere_contains_point (void)
{
  graphene_point3d_t center = GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f);
  graphene_point3d_t check;
  graphene_sphere_t *s;

  s = graphene_sphere_init (graphene_sphere_alloc (), &center, 1.f);
  g_assert_nonnull (s);

  graphene_point3d_init (&check, 0.f, 0.f, 0.f);
  g_assert_true (graphene_sphere_contains_point (s, &check));

  graphene_point3d_init (&check, 1.f, 0.f, 0.f);
  g_assert_true (graphene_sphere_contains_point (s, &check));

  graphene_point3d_init (&check, 0.f, -1.f, 0.f);
  g_assert_true (graphene_sphere_contains_point (s, &check));

  graphene_point3d_init (&check, 1.f, 1.f, 1.f);
  g_assert_false (graphene_sphere_contains_point (s, &check));

  graphene_sphere_free (s);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/sphere/init", sphere_init);
  g_test_add_func ("/sphere/contains-point", sphere_contains_point);

  return g_test_run ();
}
