#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (box_init_min_max)
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
  g_assert_true (graphene_point3d_equal (&min, &zero3));
  g_assert_true (graphene_point3d_equal (&max, &one3));

  graphene_box_init (b, NULL, NULL);
  g_assert_true (graphene_box_equal (b, graphene_box_zero ()));

  graphene_box_init_from_vec3 (b, graphene_vec3_zero (), graphene_vec3_one ());

  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);
  g_assert_true (graphene_point3d_equal (&min, &zero3));
  g_assert_true (graphene_point3d_equal (&max, &one3));

  graphene_box_init_from_vec3 (b, NULL, NULL);
  g_assert_true (graphene_box_equal (b, graphene_box_zero ()));

  graphene_box_free (b);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_init_from_points)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_init_from_vectors)
{
  graphene_point3d_t points[] = {
    GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f),
    GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f),
    GRAPHENE_POINT3D_INIT (2.f, 2.f, 2.f),
  };
  graphene_vec3_t *vectors;
  graphene_point3d_t min, max;
  graphene_box_t *b;

  vectors = g_new (graphene_vec3_t, 3);
  graphene_point3d_to_vec3 (&points[0], &vectors[0]);
  graphene_point3d_to_vec3 (&points[1], &vectors[1]);
  graphene_point3d_to_vec3 (&points[2], &vectors[2]);

  b = graphene_box_init_from_vectors (graphene_box_alloc (), 3, vectors);
  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);
  g_assert_true (graphene_point3d_equal (&min, &points[0]));
  g_assert_true (graphene_point3d_equal (&max, &points[2]));

  graphene_box_init_from_vectors (b, 1, vectors + 1);
  graphene_box_get_min (b, &min);
  graphene_box_get_max (b, &max);
  g_assert_true (graphene_point3d_equal (&min, &points[1]));
  g_assert_true (graphene_point3d_equal (&max, &points[1]));
  g_assert_true (graphene_point3d_equal (&min, &max));

  graphene_box_init_from_vectors (b, 0, NULL);
  g_assert_true (graphene_box_equal (b, graphene_box_empty ()));

  graphene_box_free (b);
  g_free (vectors);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_size)
{
  graphene_vec3_t size;

  graphene_box_get_size (graphene_box_zero (), &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  g_assert_cmpfloat (graphene_box_get_width (graphene_box_zero ()), ==, 0.f);
  g_assert_cmpfloat (graphene_box_get_height (graphene_box_one ()), ==, 1.f);
  g_assert_cmpfloat (graphene_box_get_depth (graphene_box_one_minus_one ()), ==, 2.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_center)
{
  graphene_point3d_t center;
  graphene_box_t b;

  graphene_box_init_from_box (&b, graphene_box_zero ());
  graphene_box_get_center (&b, &center);
  g_assert_true (graphene_point3d_equal (&center, &zero3));

  graphene_box_init_from_box (&b, graphene_box_one ());
  graphene_box_get_center (&b, &center);
  g_assert_true (graphene_point3d_equal (&center, &half3));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_equal)
{
  g_assert_true (graphene_box_equal (NULL, NULL));
  g_assert_false (graphene_box_equal (NULL, graphene_box_zero ()));
  g_assert_false (graphene_box_equal (graphene_box_zero (), NULL));

  g_assert_true (graphene_box_equal (graphene_box_zero (), graphene_box_zero ()));
  g_assert_false (graphene_box_equal (graphene_box_zero (), graphene_box_one ()));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_union)
{
  graphene_box_t res;

  graphene_box_union (graphene_box_zero (), graphene_box_zero (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_zero ()));

  graphene_box_union (graphene_box_zero (), graphene_box_one (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));

  graphene_box_union (graphene_box_minus_one (), graphene_box_one (), &res);
  g_assert_true (graphene_box_equal (&res, graphene_box_one_minus_one ()));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_intersection)
{
  graphene_box_t res;

  g_assert_false (graphene_box_intersection (graphene_box_empty (), graphene_box_zero (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_empty ()));

  g_assert_true (graphene_box_intersection (graphene_box_one_minus_one (), graphene_box_one (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));

  g_assert_true (graphene_box_intersection (graphene_box_infinite (), graphene_box_one (), &res));
  g_assert_true (graphene_box_equal (&res, graphene_box_one ()));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_expand_by_point)
{
  graphene_box_t b;
  graphene_vec3_t size, tmp;
  graphene_point3d_t minus_one = GRAPHENE_POINT3D_INIT (-1.f, -1.f, -1.f);

  graphene_box_init_from_box (&b, graphene_box_zero ());
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand (&b, &zero3, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand (&b, &one3, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_one ()));

  graphene_box_expand (&b, &minus_one, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_init (&tmp, 2.f, 2.f, 2.f)));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_expand_by_vector)
{
  graphene_box_t b;
  graphene_vec3_t size, tmp;
  graphene_vec3_t minus_one;

  graphene_box_init_from_box (&b, graphene_box_zero ());
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand_vec3 (&b, graphene_vec3_zero (), &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand_vec3 (&b, graphene_vec3_one (), &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_one ()));

  graphene_vec3_init (&minus_one, -1.f, -1.f, -1.f);
  graphene_box_expand_vec3 (&b, &minus_one, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_init (&tmp, 2.f, 2.f, 2.f)));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_expand_by_scalar)
{
  graphene_box_t b;
  graphene_vec3_t size, tmp;

  graphene_box_init_from_box (&b, graphene_box_zero ());
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand_scalar (&b, 0, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_zero ()));

  graphene_box_expand_scalar (&b, 1, &b);
  graphene_box_get_size (&b, &size);
  g_assert_true (graphene_vec3_equal (&size, graphene_vec3_init (&tmp, 2.f, 2.f, 2.f)));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_contains_point)
{
  graphene_box_t b;
  graphene_vec3_t points[8];
  graphene_point3d_t center;
  graphene_point3d_t check;
  unsigned int i;

  graphene_box_init_from_box (&b, graphene_box_one_minus_one ());

  graphene_box_get_center (&b, &center);
  g_assert_true (graphene_box_contains_point (&b, &center));

  graphene_box_get_vertices (&b, points);
  for (i = 0; i < 8; i++)
    {
      graphene_point3d_init_from_vec3 (&check, &points[i]);
      g_assert_true (graphene_box_contains_point (&b, &check));
    }

  graphene_point3d_init (&check, -2.f, 2.f, -2.f);
  g_assert_false (graphene_box_contains_point (&b, &check));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (box_contains_box)
{
  graphene_box_t a, b;

  graphene_box_init_from_box (&a, graphene_box_one ());
  graphene_box_init_from_box (&b, graphene_box_minus_one ());
  g_assert_false (graphene_box_contains_box (&a, &b));

  graphene_box_init_from_box (&a, graphene_box_one_minus_one ());
  graphene_box_init_from_box (&b, graphene_box_one ());
  g_assert_true (graphene_box_contains_box (&a, &b));

  graphene_box_init_from_box (&a, graphene_box_infinite ());
  g_assert_true (graphene_box_contains_box (&a, &b));

  graphene_box_init_from_box (&a, graphene_box_empty ());
  g_assert_false (graphene_box_contains_box (&a, &b));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/box/init/min-max", box_init_min_max)
  GRAPHENE_TEST_UNIT ("/box/init/from-points", box_init_from_points)
  GRAPHENE_TEST_UNIT ("/box/init/from-vectors", box_init_from_vectors)
  GRAPHENE_TEST_UNIT ("/box/size", box_size)
  GRAPHENE_TEST_UNIT ("/box/center", box_center)
  GRAPHENE_TEST_UNIT ("/box/equal", box_equal)
  GRAPHENE_TEST_UNIT ("/box/union", box_union)
  GRAPHENE_TEST_UNIT ("/box/intersection", box_intersection)
  GRAPHENE_TEST_UNIT ("/box/expand/by-point", box_expand_by_point)
  GRAPHENE_TEST_UNIT ("/box/expand/by-vector", box_expand_by_vector)
  GRAPHENE_TEST_UNIT ("/box/expand/by-scalar", box_expand_by_scalar)
  GRAPHENE_TEST_UNIT ("/box/contains/point", box_contains_point)
  GRAPHENE_TEST_UNIT ("/box/contains/box", box_contains_box)
)
