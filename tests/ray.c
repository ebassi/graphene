/* SPDX-License-Identifier: MIT */

#include <graphene.h>
#include <mutest.h>
#include <math.h>

static void
ray_init (void)
{
  graphene_ray_t *r;
  graphene_point3d_t origin, zero = GRAPHENE_POINT3D_INIT_ZERO;
  graphene_vec3_t direction;

  r = graphene_ray_init (graphene_ray_alloc (), NULL, NULL);
  graphene_ray_get_origin (r, &origin);
  graphene_ray_get_direction (r, &direction);

  mutest_expect ("initializing origin to null sets it to (0, 0, 0)",
                 mutest_bool_value (graphene_point3d_equal (&origin, &zero)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("initializing direction to null uses a zero vector",
                 mutest_bool_value (graphene_vec3_equal (&direction, graphene_vec3_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_free (r);
}

static void
ray_get_position_at (void)
{
  graphene_point3d_t one = GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f);
  graphene_point3d_t tmp;
  graphene_ray_t r;

  graphene_ray_init (&r, &one, graphene_vec3_z_axis ());

  graphene_ray_get_position_at (&r, 0.f, &tmp);
  mutest_expect ("querying a distance of 0 to return the origin",
                 mutest_bool_value (graphene_point3d_equal (&tmp, &one)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_position_at (&r, -1.f, &tmp);
  mutest_expect ("querying a negative distance to return a point behind the origin",
                 mutest_bool_value (graphene_point3d_near (&tmp, &GRAPHENE_POINT3D_INIT (1.f, 1.f, 0.f), 0.0001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_position_at (&r, 1.f, &tmp);
  mutest_expect ("querying a positive distance to return a point on the ray",
                 mutest_bool_value (graphene_point3d_near (&tmp, &GRAPHENE_POINT3D_INIT (1.f, 1.f, 2.f), 0.0001f)),
                 mutest_to_be_true,
                 NULL);
}

static void
ray_get_distance_to_point (void)
{
  graphene_ray_t r;

  graphene_ray_init (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1), graphene_vec3_z_axis ());

  mutest_expect ("closest approach from (0, 0, 0) to be sqrt(3)",
                 mutest_float_value (graphene_ray_get_distance_to_point (&r, &GRAPHENE_POINT3D_INIT (0, 0, 0))),
                 mutest_to_be_close_to, sqrtf (3.f), 0.0001,
                 NULL);

  mutest_expect ("closest approach on the origin of the ray to be zero",
                 mutest_float_value (graphene_ray_get_distance_to_point (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1))),
                 mutest_to_be_close_to, 0.0, 0.0001,
                 NULL);

  mutest_expect ("closest approach from (0, 0, 50) to be sqrt(2)",
                 mutest_float_value (graphene_ray_get_distance_to_point (&r, &GRAPHENE_POINT3D_INIT (0, 0, 50))),
                 mutest_to_be_close_to, sqrtf (2.f), 0.0001,
                 NULL);
}

static void
ray_closest_point_to_point (void)
{
  graphene_point3d_t res;
  graphene_ray_t r;

  graphene_ray_init (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1), graphene_vec3_z_axis ());

  graphene_ray_get_closest_point_to_point (&r, &GRAPHENE_POINT3D_INIT (0, 0, 0), &res);
  mutest_expect ("closest point behind the origin is the origin",
                 mutest_bool_value (graphene_point3d_near (&res, &GRAPHENE_POINT3D_INIT (1, 1, 1), 0.00001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_closest_point_to_point (&r, &GRAPHENE_POINT3D_INIT (0, 0, 50), &res);
  mutest_expect ("closest point in front of the ray to be on the ray",
                 mutest_bool_value (graphene_point3d_near (&res, &GRAPHENE_POINT3D_INIT (1, 1, 50), 0.00001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_closest_point_to_point (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1), &res);
  mutest_expect ("closest point on the ray to be the same point",
                 mutest_bool_value (graphene_point3d_near (&res, &GRAPHENE_POINT3D_INIT (1, 1, 1), 0.00001f)),
                 mutest_to_be_true,
                 NULL);
}

static void
ray_matrix_transform (void)
{
  graphene_ray_t r, res;
  graphene_matrix_t m;

  graphene_ray_init (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1), graphene_vec3_z_axis ());
  graphene_matrix_init_identity (&m);
  graphene_matrix_transform_ray (&m, &r, &res);
  mutest_expect ("transforming a ray with an identity matrix does not change the ray",
                 mutest_bool_value (graphene_ray_equal (&r, &res)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_init (&r, &GRAPHENE_POINT3D_INIT (0, 0, 0), graphene_vec3_z_axis ());
  graphene_matrix_init_rotate (&m, 90, graphene_vec3_z_axis ());
  graphene_matrix_transform_ray (&m, &r, &res);
  mutest_expect ("transforming a ray on the Z axis with a rotation on the Z axis does not change the ray",
                 mutest_bool_value (graphene_ray_equal (&r, &res)),
                 mutest_to_be_true,
                 NULL);
}

static void
ray_intersect_triangle (void)
{
  graphene_vec3_t one3;
  graphene_vec3_init (&one3, 1, 1, 1);

  graphene_ray_t r;

  graphene_triangle_t t;
  graphene_triangle_init_from_point3d (&t,
                                       &GRAPHENE_POINT3D_INIT (1, 1, 0),
                                       &GRAPHENE_POINT3D_INIT (0, 1, 1),
                                       &GRAPHENE_POINT3D_INIT (1, 0, 1));

  graphene_ray_init (&r, NULL, graphene_vec3_zero ());
  mutest_expect ("no intersection if ray's direction is zero",
                 mutest_int_value (graphene_ray_intersect_triangle (&r, &t, NULL)),
                 mutest_to_be, GRAPHENE_RAY_INTERSECTION_KIND_NONE,
                 NULL);

  graphene_ray_init (&r, NULL, &one3);
  mutest_expect ("intersection with backside faces",
                 mutest_int_value (graphene_ray_intersect_triangle (&r, &t, NULL)),
                 mutest_to_be, GRAPHENE_RAY_INTERSECTION_KIND_LEAVE,
                 NULL);

  float d;
  graphene_ray_init (&r, NULL, &one3);
  mutest_expect ("intersection without backside faces",
                 mutest_int_value (graphene_ray_intersect_triangle (&r, &t, &d)),
                 mutest_not, mutest_to_be, GRAPHENE_RAY_INTERSECTION_KIND_NONE,
                 NULL);
  mutest_expect ("intersection distance to be valid",
                 mutest_float_value (d),
                 mutest_to_be_close_to, d - 1 / 3, 0.001,
                 NULL);
}

static void
ray_suite (void)
{
  mutest_it ("can be initialized", ray_init);
  mutest_it ("can compute the position of a point on the ray", ray_get_position_at);
  mutest_it ("can compute the closest approach of a point from the ray", ray_get_distance_to_point);
  mutest_it ("can compute the closest point to a point on the ray", ray_closest_point_to_point);
  mutest_it ("can be transformed", ray_matrix_transform);
  mutest_it ("can intersect triangles", ray_intersect_triangle);
}

MUTEST_MAIN (
  mutest_describe ("graphene_ray_t", ray_suite);
)
