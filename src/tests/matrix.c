#include <glib.h>
#include <graphene.h>
#include <math.h>

#include "graphene-test-compat.h"

#define compare_matrices(m1,m2)         G_STMT_START { \
  float __me1[16], __me2[16]; \
  unsigned int __x, __y; \
  graphene_matrix_to_float ((m1), __me1); \
  graphene_matrix_to_float ((m2), __me2); \
  for (__x = 0; __x < 4; __x++) { \
    for (__y = 0; __y < 4; __y++) { \
      unsigned int __idx = __x + __y; \
      if (g_test_verbose ()) \
        g_print ("[%d][%d] ", __x, __y); \
      graphene_assert_fuzzy_matrix_equal (__x, __y, __me1[__idx], __me2[__idx], 0.0001f); \
    } \
    if (g_test_verbose ()) \
      g_print ("\n"); \
  }                                     } G_STMT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_identity)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_rotation)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_rotation_euler_quaternion)
{
/* XXX: disable on GCC < 4.9; it seems the compiler emits wrong floating
 *      point instructions with -ffast-math
 */
#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
# if GLIB_CHECK_VERSION (2, 38, 0)
  g_test_skip ("Disabled on GCC < 4.9");
# else
  if (g_test_verbose ())
    g_test_message ("Disabled on GCC < 4.9");
# endif
#else
  graphene_matrix_t m0, m1, m2;
  graphene_quaternion_t q;
  graphene_euler_t e;

  graphene_matrix_init_rotate (&m0, 60.f, graphene_vec3_x_axis ());

  graphene_euler_init (&e, 60.f, 0.f, 0.f);
  graphene_quaternion_init_from_euler (&q, &e);

  graphene_matrix_init_identity (&m1);
  graphene_matrix_rotate_euler (&m1, &e);

  graphene_matrix_init_identity (&m2);
  graphene_matrix_rotate_quaternion (&m2, &q);

  compare_matrices (&m0, &m1);
  compare_matrices (&m0, &m2);
  compare_matrices (&m1, &m2);
#endif
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_translation)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_scale)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_invert)
{
  graphene_matrix_t m;
  graphene_matrix_t identity;
  graphene_matrix_t inv;
  graphene_matrix_t res;
  graphene_point3d_t p;

  graphene_matrix_init_identity (&identity);

  graphene_matrix_init_identity (&m);
  
  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);
  
  graphene_matrix_scale (&m, 1.0f, 2.0f, 3.0f);

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);

  graphene_matrix_rotate (&m, 44, graphene_vec3_x_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);

  graphene_matrix_rotate (&m, 12, graphene_vec3_y_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);

  graphene_matrix_translate (&m,
			     graphene_point3d_init (&p, 1,2,3));

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);

  graphene_matrix_rotate (&m, 200, graphene_vec3_z_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  compare_matrices (&identity, &res);
  
  /*
  g_print ("m:\n");
  graphene_matrix_print (&m);
  g_print ("inv_m:\n");
  graphene_matrix_print (&inv);
  g_print ("m * inv_m:\n");
  graphene_matrix_print (&res);
  */
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_neutral_element)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_look_at)
{
  graphene_matrix_t m;
  graphene_vec3_t neg_z_axis;
  graphene_vec3_t dir_xz, dir_yz, dir_xyz;
  graphene_point3d_t p;
#if 0
  graphene_vec3_t eye, center, up, dist, center_plus_up;
  graphene_vec4_t res;
#endif
  graphene_matrix_t identity, identity_translated, rotated;

  graphene_matrix_init_identity (&identity);
  graphene_vec3_init (&neg_z_axis, 0, 0, -1);
  graphene_vec3_init (&dir_xz, 1, 0, -1);
  graphene_vec3_init (&dir_yz, 0, 1, -1);
  graphene_vec3_init (&dir_xyz, 1, 1, -1);

  if (g_test_verbose ())
    g_print ("look_at: eye: (0, 0, 0), center: (0, 0, -1), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &neg_z_axis,
                                graphene_vec3_y_axis());
  compare_matrices (&m, &identity);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 1), center: (0, 0, 0), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_z_axis(),
                                graphene_vec3_zero (),
                                graphene_vec3_y_axis());

  identity_translated = identity;
  graphene_matrix_translate (&identity_translated,
                             graphene_point3d_init (&p, 0, 0, -1));
  compare_matrices (&m, &identity_translated);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 0), center: (1, 0, -1), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_xz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, -45, graphene_vec3_y_axis());
  compare_matrices (&m, &rotated);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 0), center: (0, 1, -1), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_yz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, 45, graphene_vec3_x_axis());
  compare_matrices (&m, &rotated);

#if 0
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

  if (g_test_verbose ())
    g_print ("look at: eye: (%+5.3f, %+5.3f, %+5.3f), "
                   "center: (+%5.3f, %+5.3f, %+5.3f), "
                       "up: (%+.3f, %+.3f, %+.3f)\n",
             graphene_vec3_get_x (&eye), graphene_vec3_get_y (&eye), graphene_vec3_get_z (&eye),
             graphene_vec3_get_x (&center), graphene_vec3_get_y (&center), graphene_vec3_get_z (&center),
             graphene_vec3_get_x (&up), graphene_vec3_get_y (&up), graphene_vec3_get_z (&up));
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
#endif
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/matrix/identity", matrix_identity)
  GRAPHENE_TEST_UNIT ("/matrix/scale", matrix_scale)
  GRAPHENE_TEST_UNIT ("/matrix/rotation", matrix_rotation)
  GRAPHENE_TEST_UNIT ("/matrix/rotation/euler-quaternion", matrix_rotation_euler_quaternion)
  GRAPHENE_TEST_UNIT ("/matrix/translation", matrix_translation)
  GRAPHENE_TEST_UNIT ("/matrix/neutral_element", matrix_neutral_element)
  GRAPHENE_TEST_UNIT ("/matrix/look_at", matrix_look_at)
  GRAPHENE_TEST_UNIT ("/matrix/invert", matrix_invert)
)
