#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (ray_init)
{
  graphene_ray_t *r;
  graphene_point3d_t origin;
  graphene_vec3_t direction;

  r = graphene_ray_init (graphene_ray_alloc (), NULL, NULL);
  graphene_ray_get_origin (r, &origin);
  graphene_ray_get_direction (r, &direction);

  g_assert_true (graphene_point3d_equal (&origin, &zero3));
  g_assert_true (graphene_vec3_equal (&direction, graphene_vec3_zero ()));

  graphene_ray_free (r);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (ray_get_position_at)
{
  graphene_ray_t r;
  graphene_point3d_t tmp;

  graphene_ray_init (&r, &one3, graphene_vec3_z_axis ());

  if (g_test_verbose ())
    g_test_message ("On the ray's origin...");
  graphene_ray_get_position_at (&r, 0.f, &tmp);
  g_assert_true (graphene_point3d_equal (&tmp, &one3));

  if (g_test_verbose ())
    g_test_message ("Behind the ray...");
  graphene_ray_get_position_at (&r, -1.f, &tmp);
  graphene_assert_fuzzy_equals (tmp.x, 1.f, 0.0001);
  graphene_assert_fuzzy_equals (tmp.y, 1.f, 0.0001);
  graphene_assert_fuzzy_equals (tmp.z, 0.f, 0.0001);

  if (g_test_verbose ())
    g_test_message ("On the ray...");
  graphene_ray_get_position_at (&r, 1.f, &tmp);
  graphene_assert_fuzzy_equals (tmp.x, 1.f, 0.0001);
  graphene_assert_fuzzy_equals (tmp.y, 1.f, 0.0001);
  graphene_assert_fuzzy_equals (tmp.z, 2.f, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (ray_get_distance_to_point)
{
  graphene_ray_t r;
  graphene_point3d_t tmp = GRAPHENE_POINT3D_INIT (0, 0, 50);

  graphene_ray_init (&r, &one3, graphene_vec3_z_axis ());

  if (g_test_verbose ())
    g_test_message ("On the ray's origin...");
  graphene_assert_fuzzy_equals (graphene_ray_get_distance_to_point (&r, &one3), 0.f, 0.00001);

  if (g_test_verbose ())
    g_test_message ("Behind the ray...");
  graphene_assert_fuzzy_equals (graphene_ray_get_distance_to_point (&r, &zero3), sqrtf (3.f), 0.00001);

  if (g_test_verbose ())
    g_test_message ("On the ray...");
  graphene_assert_fuzzy_equals (graphene_ray_get_distance_to_point (&r, &tmp), sqrtf (2.f), 0.00001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (ray_closest_point_to_point)
{
  graphene_point3d_t tmp = GRAPHENE_POINT3D_INIT (0.f, 0.f, 50.f);
  graphene_point3d_t check = GRAPHENE_POINT3D_INIT (1.f, 1.f, 50.f);
  graphene_point3d_t res;
  graphene_ray_t r;

  graphene_ray_init (&r, &one3, graphene_vec3_z_axis ());

  if (g_test_verbose ())
    g_test_message ("Behind the ray...");
  graphene_ray_get_closest_point_to_point (&r, &zero3, &res);
  g_assert_true (graphene_point3d_near (&res, &one3, 0.00001));

  if (g_test_verbose ())
    g_test_message ("Front of the ray...");
  graphene_ray_get_closest_point_to_point (&r, &tmp, &res);
  g_assert_true (graphene_point3d_near (&res, &check, 0.00001));

  if (g_test_verbose ())
    g_test_message ("On the ray...");
  graphene_ray_get_closest_point_to_point (&r, &one3, &res);
  g_assert_true (graphene_point3d_near (&res, &one3, 0.00001));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (ray_matrix_transform)
{
  graphene_ray_t r, res;
  graphene_matrix_t m;

  if (g_test_verbose ())
    g_test_message ("Identity matrix...");
  graphene_ray_init (&r, &one3, graphene_vec3_z_axis ());
  graphene_matrix_init_identity (&m);
  graphene_matrix_transform_ray (&m, &r, &res);
  g_assert_true (graphene_ray_equal (&r, &res));

  if (g_test_verbose ())
    g_test_message ("Rotation matrix: rotateZ(90deg)");
  graphene_ray_init (&r, &zero3, graphene_vec3_z_axis ());
  graphene_matrix_init_rotate (&m, 90, graphene_vec3_z_axis ());
  graphene_matrix_transform_ray (&m, &r, &res);
  g_assert_true (graphene_ray_equal (&r, &res));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/ray/init", ray_init)
  GRAPHENE_TEST_UNIT ("/ray/get-position-at", ray_get_position_at)
  GRAPHENE_TEST_UNIT ("/ray/get-distance-to-point", ray_get_distance_to_point)
  GRAPHENE_TEST_UNIT ("/ray/closest-point-to-point", ray_closest_point_to_point)
  GRAPHENE_TEST_UNIT ("/ray/matrix-transform", ray_matrix_transform)
)
