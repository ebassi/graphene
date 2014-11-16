#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
box_init_min_max (void)
{
  graphene_point3d_t points[] = {
    GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f),
    GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f)
  };
  graphene_point3d_t min, max;
  graphene_box_t *b;

  b = graphene_box_init (graphene_box_alloc (), &points[0], &points[1]);
  g_assert_nonnull (b);

  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);

  g_assert_true (graphene_point3d_equal (&min, &points[0]));
  g_assert_true (graphene_point3d_equal (&max, &points[1]));

  graphene_box_init (b, NULL, NULL);
  g_assert_true (graphene_box_equal (b, graphene_box_zero ()));

  graphene_box_free (b);
}

static void
box_init_from_points (void)
{
  graphene_point3d_t points[] = {
    GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f),
    GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f),
    GRAPHENE_POINT3D_INIT (2.f, 2.f, 2.f),
  };
  graphene_point3d_t min, max;
  graphene_box_t *b;

  b = graphene_box_init_from_points (graphene_box_alloc (), 3, points);
  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);
  g_assert_true (graphene_point3d_equal (&min, &points[0]));
  g_assert_true (graphene_point3d_equal (&max, &points[2]));

  graphene_box_init_from_points (b, 1, points + 1);
  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);
  g_assert_true (graphene_point3d_equal (&min, &points[1]));
  g_assert_true (graphene_point3d_equal (&max, &points[1]));
  g_assert_true (graphene_point3d_equal (&min, &max));

  graphene_box_init_from_points (b, 0, NULL);
  g_assert_true (graphene_box_equal (b, graphene_box_empty ()));

  graphene_box_free (b);
}

static void
box_size (void)
{
  graphene_vec3_t size;

  graphene_box_get_size (graphene_box_zero (), &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  g_assert_cmpfloat (graphene_box_get_width (graphene_box_zero ()), ==, 0.f);
  g_assert_cmpfloat (graphene_box_get_height (graphene_box_one ()), ==, 1.f);
  g_assert_cmpfloat (graphene_box_get_depth (graphene_box_one_minus_one ()), ==, 2.f);
}

static void
box_center (void)
{
  const graphene_point3d_t zero = GRAPHENE_POINT3D_INIT_ZERO;
  const graphene_point3d_t half = GRAPHENE_POINT3D_INIT (.5f, .5f, .5f);
  graphene_point3d_t center;
  graphene_box_t b;

  graphene_box_init_from_box (&b, graphene_box_zero ());
  graphene_box_get_center (&b, &center);
  g_assert_true (graphene_point3d_equal (&center, &zero));

  graphene_box_init_from_box (&b, graphene_box_one ());
  graphene_box_get_center (&b, &center);
  g_assert_true (graphene_point3d_equal (&center, &half));
}

static void
box_equal (void)
{
  g_assert_true (graphene_box_equal (NULL, NULL));
  g_assert_false (graphene_box_equal (NULL, graphene_box_zero ()));
  g_assert_false (graphene_box_equal (graphene_box_zero (), NULL));

  g_assert_true (graphene_box_equal (graphene_box_zero (), graphene_box_zero ()));
  g_assert_false (graphene_box_equal (graphene_box_zero (), graphene_box_one ()));
}

static void
box_union (void)
{
  graphene_box_t res;

  graphene_box_union (graphene_box_zero (), graphene_box_zero (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_zero ()));

  graphene_box_union (graphene_box_zero (), graphene_box_one (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));

  graphene_box_union (graphene_box_minus_one (), graphene_box_one (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_one_minus_one ()));
}

static void
box_intersection (void)
{
  graphene_box_t res;

  g_assert_false (graphene_box_intersection (graphene_box_empty (), graphene_box_zero (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_empty ()));

  g_assert_true (graphene_box_intersection (graphene_box_one_minus_one (), graphene_box_one (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));

  g_assert_true (graphene_box_intersection (graphene_box_infinite (), graphene_box_one (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/box/init/min-max", box_init_min_max);
  g_test_add_func ("/box/init/from-points", box_init_from_points);
  g_test_add_func ("/box/size", box_size);
  g_test_add_func ("/box/center", box_center);
  g_test_add_func ("/box/equal", box_equal);
  g_test_add_func ("/box/union", box_union);
  g_test_add_func ("/box/intersection", box_intersection);

  return g_test_run ();
}
