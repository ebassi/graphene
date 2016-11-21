#include <glib.h>
#include <graphene.h>
#include <math.h>

#include "graphene-test-compat.h"

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

  v[0]  = 1.f; v[4]  = 0.f; v[8]  = 0.f; v[12] = 0.f;
  v[1]  = 0.f; v[5]  = 1.f; v[9]  = 0.f; v[13] = 0.f;
  v[2]  = 0.f; v[6]  = 0.f; v[10] = 1.f; v[14] = 0.f;
  v[3]  = 0.f; v[7]  = 0.f; v[11] = 0.f; v[15] = 1.f;

  graphene_matrix_init_from_float (&m, v);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_init_from_vec4 (&m,
                                  graphene_vec4_x_axis (),
                                  graphene_vec4_y_axis (),
                                  graphene_vec4_z_axis (),
                                  graphene_vec4_w_axis ());
  g_assert_true (graphene_matrix_is_identity (&m));
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

  graphene_assert_fuzzy_matrix_equal (&m, &m2, 0.0001f);

  graphene_matrix_rotate (&m, 15.0f, graphene_vec3_x_axis ());
  graphene_matrix_init_rotate (&m2, 60.0f, graphene_vec3_x_axis ());

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  graphene_assert_fuzzy_matrix_equal (&m, &m2, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_rotation_euler_quaternion)
{
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

  graphene_assert_fuzzy_matrix_equal (&m0, &m1, 0.0001f);
  graphene_assert_fuzzy_matrix_equal (&m0, &m2, 0.0001f);
  graphene_assert_fuzzy_matrix_equal (&m1, &m2, 0.0001f);
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

  graphene_assert_fuzzy_matrix_equal (&m, &m2, 0.0001f);
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

  graphene_assert_fuzzy_matrix_equal (&m, &m2, 0.0001f);

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
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);
  
  graphene_matrix_scale (&m, 1.0f, 2.0f, 3.0f);

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);

  graphene_matrix_rotate (&m, 44, graphene_vec3_x_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);

  graphene_matrix_rotate (&m, 12, graphene_vec3_y_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);

  graphene_matrix_translate (&m, graphene_point3d_init (&p, 1.f, 2.f, 3.f));

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);

  graphene_matrix_rotate (&m, 200, graphene_vec3_z_axis ());

  graphene_matrix_inverse (&m , &inv);
  graphene_matrix_multiply (&m, &inv, &res);
  graphene_assert_fuzzy_matrix_equal (&identity, &res, 0.0001f);
  
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
  graphene_assert_fuzzy_matrix_equal (&m, &identity, 0.0001f);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 1), center: (0, 0, 0), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_z_axis(),
                                graphene_vec3_zero (),
                                graphene_vec3_y_axis());

  identity_translated = identity;
  graphene_matrix_translate (&identity_translated,
                             graphene_point3d_init (&p, 0, 0, -1));
  graphene_assert_fuzzy_matrix_equal (&m, &identity_translated, 0.0001f);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 0), center: (1, 0, -1), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_xz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, -45, graphene_vec3_y_axis());
  graphene_assert_fuzzy_matrix_equal (&m, &rotated, 0.0001f);

  if (g_test_verbose ())
    g_print ("look at: eye: (0, 0, 0), center: (0, 1, -1), up: (0, 1, 0)\n");
  graphene_matrix_init_look_at (&m,
                                graphene_vec3_zero (),
                                &dir_yz,
                                graphene_vec3_y_axis());
  graphene_matrix_init_rotate (&rotated, 45, graphene_vec3_x_axis());
  graphene_assert_fuzzy_matrix_equal (&m, &rotated, 0.0001f);

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

GRAPHENE_TEST_UNIT_BEGIN (matrix_2d_identity)
{
  graphene_matrix_t m1;

  graphene_matrix_init_from_2d (&m1,
                                1, 0,
                                0, 1,
                                0, 0);
  g_assert_true (graphene_matrix_is_2d (&m1));
  g_assert_true (graphene_matrix_is_identity (&m1));

  graphene_matrix_init_from_2d (&m1,
                                1, 1,
                                1, 1,
                                0, 0);
  g_assert_true (graphene_matrix_is_2d (&m1));
  g_assert_false (graphene_matrix_is_identity (&m1));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_2d_transforms)
{
  graphene_matrix_t m1, m2;
  double xx, xy, yx, yy, x_0, y_0;

  graphene_matrix_init_identity (&m1);
  graphene_matrix_scale (&m1, 2.0, 2.0, 1.0);
  graphene_matrix_translate (&m1, &GRAPHENE_POINT3D_INIT (0.5, 0.5, 0.0));
  if (g_test_verbose ())
    {
      g_test_message ("m1 -> translate(0.5, 0.5, 0.0) -> scale(2.0, 2.0)");
      graphene_matrix_print (&m1);
    }

  g_assert_true (graphene_matrix_is_2d (&m1));

  graphene_matrix_init_from_2d (&m2,
                                2.0, 0.0,
                                0.0, 2.0,
                                0.5, 0.5);
  if (g_test_verbose ())
    {
      g_test_message ("m2 -> [ 2.0, 0.0 | 0.0, 2.0 | 0.5, 0.5 ]");
      graphene_matrix_print (&m2);
    }

  graphene_assert_fuzzy_matrix_equal (&m1, &m2, 0.0001);

  graphene_matrix_to_2d (&m1, &xx, &yx, &xy, &yy, &x_0, &y_0);
  graphene_assert_fuzzy_equals (xx, 2.0, 0.0001);
  graphene_assert_fuzzy_equals (yx, 0.0, 0.0001);
  graphene_assert_fuzzy_equals (xy, 0.0, 0.0001);
  graphene_assert_fuzzy_equals (yy, 2.0, 0.0001);
  graphene_assert_fuzzy_equals (x_0, 0.5, 0.0001);
  graphene_assert_fuzzy_equals (y_0, 0.5, 0.0001);

  graphene_matrix_init_identity (&m1);
  graphene_matrix_translate (&m1, &GRAPHENE_POINT3D_INIT (50, 50, 0));
  graphene_matrix_rotate_z (&m1, 45.0);
  graphene_matrix_translate (&m1, &GRAPHENE_POINT3D_INIT (-50, -50, 0));
  if (g_test_verbose ())
    {
      g_test_message ("m1 -> translate(50,50) -> rotate(45deg) -> translate(-50, -50)");
      graphene_matrix_print (&m1);
    }

  g_assert_true (graphene_matrix_is_2d (&m1));

  graphene_matrix_init_identity (&m1);
  graphene_matrix_perspective (&m1, 500, &m1);
  graphene_matrix_rotate_y (&m1, 50.0);
  if (g_test_verbose ())
    {
      g_test_message ("m1 -> perspective(500) -> rotateY(50deg)");
      graphene_matrix_print (&m1);
    }

  g_assert_false (graphene_matrix_is_2d (&m1));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_2d_round_trip)
{
  graphene_matrix_t m1, m2;
  double xx, xy, yx, yy, x_0, y_0;

  graphene_matrix_init_from_2d (&m1,
                                2.0, 0.0,
                                0.0, 2.0,
                                0.5, 0.5);
  g_assert_true (graphene_matrix_is_2d (&m1));

  graphene_matrix_to_2d (&m1, &xx, &yx, &xy, &yy, &x_0, &y_0);
  graphene_assert_fuzzy_equals (xx, 2.0, 0.0001);
  graphene_assert_fuzzy_equals (yx, 0.0, 0.0001);
  graphene_assert_fuzzy_equals (xy, 0.0, 0.0001);
  graphene_assert_fuzzy_equals (yy, 2.0, 0.0001);
  graphene_assert_fuzzy_equals (x_0, 0.5, 0.0001);
  graphene_assert_fuzzy_equals (y_0, 0.5, 0.0001);

  graphene_matrix_init_from_2d (&m2, xx, yx, xy, yy, x_0, y_0);
  graphene_assert_fuzzy_matrix_equal (&m1, &m2, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_interpolate)
{
  graphene_matrix_t m1, m2, m3, mr;

  graphene_matrix_init_identity (&m1);
  graphene_matrix_init_identity (&m2);

  graphene_matrix_translate (&m2, &GRAPHENE_POINT3D_INIT (100.f, 100.f, 100.f));
  g_assert_true (!graphene_matrix_is_2d (&m2));

  graphene_matrix_interpolate (&m1, &m2, 0.0, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m1, 0.1);

  graphene_matrix_interpolate (&m1, &m2, 1.0, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m2, 0.1);

  graphene_matrix_init_translate (&m3, &GRAPHENE_POINT3D_INIT (50.f, 50.f, 50.f));
  graphene_matrix_interpolate (&m1, &m2, 0.5, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m3, 0.1);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_2d_interpolate)
{
  graphene_matrix_t m1, m2, m3, mr;

  graphene_matrix_init_identity (&m1);
  graphene_matrix_init_identity (&m2);

  graphene_matrix_translate (&m2, &GRAPHENE_POINT3D_INIT (100.f, 100.f, 0.f));

  g_assert_true (graphene_matrix_is_2d (&m1));
  g_assert_true (graphene_matrix_is_2d (&m2));

  graphene_matrix_interpolate (&m1, &m2, 0.0, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m1, 0.01);

  graphene_matrix_interpolate (&m1, &m2, 1.0, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m2, 0.01);

  graphene_matrix_init_translate (&m3, &GRAPHENE_POINT3D_INIT (50.f, 50.f, 0.f));
  graphene_matrix_interpolate (&m1, &m2, 0.5, &mr);
  graphene_assert_fuzzy_matrix_equal (&mr, &m3, 0.01);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (matrix_2d_transform_bound)
{
  graphene_matrix_t m1, m2;
  graphene_rect_t r, r2, res;

  graphene_matrix_init_identity (&m1);
  graphene_matrix_init_translate (&m2, &GRAPHENE_POINT3D_INIT (100.f, 100.f, 0.f));

  graphene_rect_init (&r, 0.f, 0.f, 50.f, 50.f);
  graphene_rect_init (&r2, 100.f, 100.f, 50.f, 50.f);

  graphene_matrix_transform_bounds (&m1, &r, &res);
  graphene_assert_fuzzy_rect_equal (&res, &r, 0.01);

  graphene_matrix_transform_bounds (&m2, &r, &res);
  graphene_assert_fuzzy_rect_equal (&res, &r2, 0.01);
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
  GRAPHENE_TEST_UNIT ("/matrix/interpolate", matrix_interpolate)
  GRAPHENE_TEST_UNIT ("/matrix/2d/identity", matrix_2d_identity)
  GRAPHENE_TEST_UNIT ("/matrix/2d/transforms", matrix_2d_transforms)
  GRAPHENE_TEST_UNIT ("/matrix/2d/round-trip", matrix_2d_round_trip)
  GRAPHENE_TEST_UNIT ("/matrix/2d/interpolate", matrix_2d_interpolate)
  GRAPHENE_TEST_UNIT ("/matrix/2d/transform_bound", matrix_2d_transform_bound)
)
