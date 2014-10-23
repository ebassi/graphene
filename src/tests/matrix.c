#include <glib.h>
#include <graphene.h>
#include <math.h>

#include "graphene-test-compat.h"

static void
compare_matrices (const graphene_matrix_t * m1,
                  const graphene_matrix_t * m2)
{
  int x, y;

  for (x = 0; x < 4; x++)
    {
      for (y = 0; y < 4; y++)
        {
          graphene_assert_fuzzy_equals (graphene_matrix_get_value (m1, x, y),
                                        graphene_matrix_get_value (m2, x, y),
                                        0.0001f);
        }
    }
}

static void
matrix_identity (void)
{
  graphene_matrix_t m;
  float v[16];

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  g_assert_cmpfloat (graphene_matrix_get_value (&m, 0, 0), ==, 1.f);
  g_assert_cmpfloat (graphene_matrix_get_value (&m, 1, 1), ==, 1.f);
  g_assert_cmpfloat (graphene_matrix_get_value (&m, 2, 2), ==, 1.f);
  g_assert_cmpfloat (graphene_matrix_get_value (&m, 3, 3), ==, 1.f);

  graphene_matrix_to_float (&m, v);
  g_assert_cmpfloat (v[0],  ==, 1.0f);
  g_assert_cmpfloat (v[5],  ==, 1.0f);
  g_assert_cmpfloat (v[10], ==, 1.0f);

  graphene_matrix_scale (&m, 2.0f, 2.0f, 2.0f);
  g_assert_false (graphene_matrix_is_identity (&m));
}

static void
matrix_rotation (void)
{
  graphene_matrix_t m;
  graphene_matrix_t m2;

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_rotate (&m, 45.0f, graphene_vec3_x_axis ());
  graphene_matrix_init_rotate (&m2, 45.0f, graphene_vec3_x_axis ());

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);

  graphene_matrix_rotate (&m, 15.0f, graphene_vec3_x_axis ());
  graphene_matrix_init_rotate (&m2, 60.0f, graphene_vec3_x_axis ());

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);
}

static void
matrix_translation (void)
{
  graphene_matrix_t m;
  graphene_matrix_t m2;

  graphene_point3d_t trans =
      GRAPHENE_POINT3D_INIT (1.0f, 2.0f, 3.0f);

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_translate (&m, &trans);
  graphene_matrix_init_translate (&m2, &trans);

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);
}

static void
matrix_scale (void)
{
  graphene_matrix_t m;
  graphene_matrix_t m2;

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_scale (&m, 1.0f, 2.0f, 3.0f);
  graphene_matrix_init_scale (&m2, 1.0f, 2.0f, 3.0f);

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);

  g_assert_cmpfloat (graphene_matrix_get_x_scale (&m), ==, graphene_matrix_get_x_scale (&m2));
  g_assert_cmpfloat (graphene_matrix_get_y_scale (&m), ==, graphene_matrix_get_y_scale (&m2));
  g_assert_cmpfloat (graphene_matrix_get_z_scale (&m), ==, graphene_matrix_get_z_scale (&m2));
}

static void
matrix_neutral_element (void)
{
  graphene_matrix_t identity;
  graphene_matrix_t scale;
  graphene_matrix_t translation;
  graphene_matrix_t rotation;

  graphene_point3d_t null_vector = GRAPHENE_POINT3D_INIT (0,0,0);

  graphene_matrix_init_identity (&identity);
  g_assert_true (graphene_matrix_is_identity (&identity));

  graphene_matrix_scale (&identity, 1.0f, 1.0f, 1.0f);
  g_assert_true (graphene_matrix_is_identity (&identity));
  graphene_matrix_init_scale (&scale, 1.0f, 1.0f, 1.0f);
  g_assert_true (graphene_matrix_is_identity (&scale));

  graphene_matrix_rotate (&identity, 0.0f, graphene_vec3_z_axis ());
  g_assert_true (graphene_matrix_is_identity (&identity));
  graphene_matrix_init_rotate (&rotation, 0.0f, graphene_vec3_z_axis ());
  g_assert_true (graphene_matrix_is_identity (&rotation));

  graphene_matrix_translate (&identity, &null_vector);
  g_assert_true (graphene_matrix_is_identity (&identity));
  graphene_matrix_init_translate (&translation, &null_vector);
  g_assert_true (graphene_matrix_is_identity (&translation));
}

static void
matrix_look_at (void)
{
  graphene_matrix_t m;
  graphene_vec3_t neg_z_axis;
  graphene_vec3_t dir_xz, dir_yz, dir_xyz;
  graphene_vec3_t eye, center, up, dist, center_plus_up;
  graphene_point3d_t p;
  graphene_vec4_t res;
  graphene_matrix_t identity, identity_translated, rotated;

  graphene_matrix_init_identity (&identity);
  graphene_vec3_init (&neg_z_axis, 0, 0, -1);
  graphene_vec3_init (&dir_xz, 1, 0, -1);
  graphene_vec3_init (&dir_yz, 0, 1, -1);
  graphene_vec3_init (&dir_xyz, 1, 1, -1);

  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &neg_z_axis,
                                graphene_vec3_y_axis());
  compare_matrices (&m, &identity);

  graphene_matrix_init_look_at (&m,
                                graphene_vec3_z_axis(),
                                graphene_vec3_zero (),
                                graphene_vec3_y_axis());

  identity_translated = identity;
  graphene_matrix_translate (&identity_translated,
                             graphene_point3d_init (&p, 0, 0, -1));
  compare_matrices (&m, &identity_translated);

  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_xz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, 45, graphene_vec3_y_axis());
  compare_matrices (&m, &rotated);

  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_yz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, -45, graphene_vec3_x_axis());
  compare_matrices (&m, &rotated);

  graphene_vec3_init (&eye,
                      g_random_double_range (-10000, 10000),
                      g_random_double_range (-10000, 10000),
                      g_random_double_range (-10000, 10000));

  graphene_vec3_init (&center,
                      g_random_double_range (-10000, 10000),
                      g_random_double_range (-10000, 10000),
                      g_random_double_range (-10000, 10000));

  graphene_vec3_init (&up,
                      g_random_double_range (-1, 1),
                      g_random_double_range (-1, 1),
                      g_random_double_range (-1, 1));
  graphene_vec3_normalize (&up, &up);

  graphene_matrix_init_look_at (&m, &eye, &center, &up);
  graphene_vec4_init_from_vec3 (&res, &center, 1);
  graphene_matrix_transform_vec4 (&m, &res, &res);

  graphene_vec3_subtract (&center, &eye, &dist);

  graphene_assert_fuzzy_equals (graphene_vec4_get_x (&res), 0, 0.01f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_y (&res), 0, 0.01f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_z (&res), -graphene_vec3_length (&dist), 0.01f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_w (&res), 1, 0.01f);

  graphene_vec3_add (&center, &up, &center_plus_up);
  graphene_vec3_subtract (&center_plus_up, &eye, &dist);
  graphene_vec4_init_from_vec3 (&res, &center_plus_up, 1);
  graphene_matrix_transform_vec4 (&m, &res, &res);

  graphene_assert_fuzzy_equals (graphene_vec4_get_x (&res), 0, 0.01f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), <=, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), >=, 0.f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_z (&res), -graphene_vec3_length (&dist), 0.01f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_w (&res), 1, 0.01f);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/matrix/identity", matrix_identity);
  g_test_add_func ("/matrix/scale", matrix_scale);
  g_test_add_func ("/matrix/rotation", matrix_rotation);
  g_test_add_func ("/matrix/translation", matrix_translation);
  g_test_add_func ("/matrix/neutral_element", matrix_neutral_element);
  g_test_add_func ("/matrix/look_at", matrix_look_at);

  return g_test_run ();
}
