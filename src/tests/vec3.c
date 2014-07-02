#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
vectors_vec3_const (void)
{
  const graphene_vec3_t *vec3;

  vec3 = graphene_vec3_zero ();
  g_assert_nonnull (vec3);
  g_assert_true (vec3 == graphene_vec3_zero ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_length (vec3), ==, 0.f);

  vec3 = graphene_vec3_one ();
  g_assert_nonnull (vec3);
  g_assert_true (vec3 == graphene_vec3_one ());
  g_assert_true (vec3 != graphene_vec3_zero ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 1.f);

  vec3 = graphene_vec3_x_axis ();
  g_assert_nonnull (vec3);
  g_assert_true (vec3 == graphene_vec3_x_axis ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 0.f);

  vec3 = graphene_vec3_y_axis ();
  g_assert_nonnull (vec3);
  g_assert_true (vec3 == graphene_vec3_y_axis ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 0.f);

  vec3 = graphene_vec3_z_axis ();
  g_assert_nonnull (vec3);
  g_assert_true (vec3 == graphene_vec3_z_axis ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 1.f);
}

static void
vectors_vec3_init (void)
{
  graphene_vec3_t *vec3 = graphene_vec3_alloc ();
  const float v[3] = { 1.f, 2.f, 3.f };
  float v_out[3];

  graphene_vec3_init (vec3, 0.5f, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 0.5f);

  graphene_vec3_init (vec3, 1.0f, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, graphene_vec3_get_y (vec3));
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, graphene_vec3_get_z (vec3));

  graphene_vec3_init_from_vec3 (vec3, graphene_vec3_one ());
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, 1.f);

  graphene_vec3_init_from_float (vec3, v);
  g_assert_cmpfloat (graphene_vec3_get_x (vec3), ==, v[0]);
  g_assert_cmpfloat (graphene_vec3_get_y (vec3), ==, v[1]);
  g_assert_cmpfloat (graphene_vec3_get_z (vec3), ==, v[2]);

  graphene_vec3_to_float (vec3, v_out);
  g_assert_cmpfloat (v_out[0], ==, v[0]);
  g_assert_cmpfloat (v_out[1], ==, v[1]);
  g_assert_cmpfloat (v_out[2], ==, v[2]);

  graphene_vec3_free (vec3);
}

static void
vectors_vec3_ops_add (void)
{
  graphene_vec3_t a, b, res;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  graphene_vec3_init (&b, 3.f, 4.f, 5.f);

  graphene_vec3_add (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 4.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 6.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 8.f);
}

static void
vectors_vec3_ops_sub (void)
{
  graphene_vec3_t a, b, res;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  graphene_vec3_init (&b, 3.f, 4.f, 5.f);

  graphene_vec3_subtract (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, -2.f);
}

static void
vectors_vec3_ops_mul (void)
{
  graphene_vec3_t a, b, res;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  graphene_vec3_init (&b, 3.f, 4.f, 5.f);

  graphene_vec3_multiply (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==,  3.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==,  8.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 15.f);
}

static void
vectors_vec3_ops_div (void)
{
  graphene_vec3_t a, b, res;

  graphene_vec3_init (&a, 6.f, 4.f, 2.f);
  graphene_vec3_init (&b, 3.f, 2.f, 1.f);

  graphene_vec3_divide (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 2.f);
}

static void
vectors_vec3_ops_dot (void)
{
  graphene_vec3_t a, b;
  float dot, check;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  graphene_vec3_init (&b, 2.f, 3.f, 4.f);

  dot = graphene_vec3_dot (&a, &b);
  check = 1.f * 2.f + 2.f * 3.f + 3.f * 4.f;
  graphene_assert_fuzzy_equals (dot, check, 0.0001f);
}

static void
vectors_vec3_ops_cross (void)
{
  graphene_vec3_t a, b, res;
  float cross_x, cross_y, cross_z;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  graphene_vec3_init (&b, 2.f, 3.f, 4.f);

  graphene_vec3_cross (&a, &b, &res);

  cross_x = 2.f * 4.f - 3.f * 3.f;
  cross_y = 3.f * 2.f - 1.f * 4.f;
  cross_z = 1.f * 3.f - 2.f * 2.f;

  graphene_assert_fuzzy_equals (graphene_vec3_get_x (&res), cross_x, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec3_get_y (&res), cross_y, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec3_get_z (&res), cross_z, 0.0001f);
}

static void
vectors_vec3_length (void)
{
  graphene_vec3_t a;
  float len, check;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);

  len = graphene_vec3_length (&a);
  check = sqrtf ((1.f * 1.f) + (2.f * 2.f) + (3.f * 3.f));

  graphene_assert_fuzzy_equals (len, check, 0.0001f);
}

static void
vectors_vec3_normalize (void)
{
  graphene_vec3_t a, b;
  float len;

  graphene_vec3_init (&a, 1.f, 2.f, 3.f);
  len = graphene_vec3_length (&a);

  graphene_vec3_normalize (&a, &b);

  graphene_assert_fuzzy_equals (graphene_vec3_get_x (&b), graphene_vec3_get_x (&a) / len, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec3_get_y (&b), graphene_vec3_get_y (&a) / len, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec3_get_z (&b),graphene_vec3_get_z (&a) / len, 0.0001f);
}

static void
vectors_vec3_compare (void)
{
  const graphene_vec3_t *zero = graphene_vec3_zero ();
  const graphene_vec3_t *one = graphene_vec3_one ();
  graphene_vec3_t a, b;
  graphene_vec3_t res;

  graphene_vec3_min (zero, one, &res);
  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 0.f);

  graphene_vec3_max (zero, one, &res);
  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 1.f);

  graphene_vec3_init (&a, 5.f, 10.f, 8.f);
  graphene_vec3_init (&b, 2.f, 12.f, 8.f);

  graphene_vec3_min (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 10.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 8.f);

  graphene_vec3_max (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec3_get_x (&res), ==, 5.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&res), ==, 12.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&res), ==, 8.f);
}

static void
vectors_vec3_conversion (void)
{
  graphene_vec3_t v;

  graphene_vec3_init (&v, 1.f, 2.f, 3.f);

  {
    graphene_vec2_t xy;

    graphene_vec3_get_xy (&v, &xy);
    g_assert_cmpfloat (graphene_vec2_get_x (&xy), ==, 1.f);
    g_assert_cmpfloat (graphene_vec2_get_y (&xy), ==, 2.f);
  }

  {
    graphene_vec3_t xy0;

    graphene_vec3_get_xy0 (&v, &xy0);
    g_assert_cmpfloat (graphene_vec3_get_x (&xy0), ==, graphene_vec3_get_x (&v));
    g_assert_cmpfloat (graphene_vec3_get_y (&xy0), ==, graphene_vec3_get_y (&v));
    g_assert_cmpfloat (graphene_vec3_get_z (&xy0), !=, graphene_vec3_get_z (&v));
    g_assert_cmpfloat (graphene_vec3_get_z (&xy0), ==, 0.f);
  }

  {
    graphene_vec4_t xyz0, xyz1, xyzw;

    graphene_vec3_get_xyz0 (&v, &xyz0);
    g_assert_cmpfloat (graphene_vec4_get_x (&xyz0), ==, graphene_vec3_get_x (&v));
    g_assert_cmpfloat (graphene_vec4_get_y (&xyz0), ==, graphene_vec3_get_y (&v));
    g_assert_cmpfloat (graphene_vec4_get_z (&xyz0), ==, graphene_vec3_get_z (&v));
    g_assert_cmpfloat (graphene_vec4_get_w (&xyz0), ==, 0.f);

    graphene_vec3_get_xyz1 (&v, &xyz1);
    g_assert_cmpfloat (graphene_vec4_get_x (&xyz1), ==, graphene_vec3_get_x (&v));
    g_assert_cmpfloat (graphene_vec4_get_y (&xyz1), ==, graphene_vec3_get_y (&v));
    g_assert_cmpfloat (graphene_vec4_get_z (&xyz1), ==, graphene_vec3_get_z (&v));
    g_assert_cmpfloat (graphene_vec4_get_w (&xyz1), ==, 1.f);

    graphene_vec3_get_xyzw (&v, 4.f, &xyzw);
    g_assert_cmpfloat (graphene_vec4_get_x (&xyzw), ==, graphene_vec3_get_x (&v));
    g_assert_cmpfloat (graphene_vec4_get_y (&xyzw), ==, graphene_vec3_get_y (&v));
    g_assert_cmpfloat (graphene_vec4_get_z (&xyzw), ==, graphene_vec3_get_z (&v));
    g_assert_cmpfloat (graphene_vec4_get_w (&xyzw), ==, 4.f);
  }
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec3/const", vectors_vec3_const);
  g_test_add_func ("/vectors/vec3/init", vectors_vec3_init);
  g_test_add_func ("/vectors/vec3/operations/add", vectors_vec3_ops_add);
  g_test_add_func ("/vectors/vec3/operations/sub", vectors_vec3_ops_sub);
  g_test_add_func ("/vectors/vec3/operations/mul", vectors_vec3_ops_mul);
  g_test_add_func ("/vectors/vec3/operations/div", vectors_vec3_ops_div);
  g_test_add_func ("/vectors/vec3/operations/dot", vectors_vec3_ops_dot);
  g_test_add_func ("/vectors/vec3/operations/cross", vectors_vec3_ops_cross);
  g_test_add_func ("/vectors/vec3/length", vectors_vec3_length);
  g_test_add_func ("/vectors/vec3/normalize", vectors_vec3_normalize);
  g_test_add_func ("/vectors/vec3/compare", vectors_vec3_compare);
  g_test_add_func ("/vectors/vec3/conversion", vectors_vec3_conversion);

  return g_test_run ();
}
