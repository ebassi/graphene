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
  mutest_expect ("querying the origin to return the origin",
                 mutest_bool_value (graphene_point3d_equal (&tmp, &one)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_position_at (&r, -1.f, &tmp);
  mutest_expect ("querying a point behind the origin to work",
                 mutest_bool_value (graphene_point3d_near (&tmp, &GRAPHENE_POINT3D_INIT (1.f, 1.f, 0.f), 0.0001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_ray_get_position_at (&r, 1.f, &tmp);
  mutest_expect ("querying a point on the ray to work",
                 mutest_bool_value (graphene_point3d_near (&tmp, &GRAPHENE_POINT3D_INIT (1.f, 1.f, 2.f), 0.0001f)),
                 mutest_to_be_true,
                 NULL);
}

static void
ray_get_distance_to_point (void)
{
  graphene_ray_t r;

  graphene_ray_init (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1), graphene_vec3_z_axis ());

  mutest_expect ("distance on the origin to be zero",
                 mutest_float_value (graphene_ray_get_distance_to_point (&r, &GRAPHENE_POINT3D_INIT (1, 1, 1))),
                 mutest_to_be_close_to, 0.0, 0.0001,
                 NULL);

  mutest_expect ("distance to a point behind the ray to be sqrt(3)",
                 mutest_float_value (graphene_ray_get_distance_to_point (&r, &GRAPHENE_POINT3D_INIT (0, 0, 0))),
                 mutest_to_be_close_to, sqrtf (3.f), 0.0001,
                 NULL);

  mutest_expect ("distance to a point in front of the ray to be sqrt(2)",
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
ray_suite (void)
{
  mutest_it ("initialize", ray_init);
  mutest_it ("gets position", ray_get_position_at);
  mutest_it ("gets distance to point", ray_get_distance_to_point);
  mutest_it ("gets closest point to point", ray_closest_point_to_point);
  mutest_it ("can be transformed", ray_matrix_transform);
}

MUTEST_MAIN (
  mutest_describe ("graphene_ray_t", ray_suite);
)
