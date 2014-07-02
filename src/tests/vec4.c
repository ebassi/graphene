#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
vectors_vec4_const (void)
{
  const graphene_vec4_t *vec4;

  vec4 = graphene_vec4_zero ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_zero ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_length (vec4), ==, 0.f);

  vec4 = graphene_vec4_one ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_one ());
  g_assert_true (vec4 != graphene_vec4_zero ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 1.f);

  vec4 = graphene_vec4_x_axis ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_x_axis ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);

  vec4 = graphene_vec4_y_axis ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_y_axis ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);

  vec4 = graphene_vec4_z_axis ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_z_axis ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);

  vec4 = graphene_vec4_w_axis ();
  g_assert_nonnull (vec4);
  g_assert_true (vec4 == graphene_vec4_w_axis ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 1.f);
}

static void
vectors_vec4_init (void)
{
  graphene_vec4_t *vec4 = graphene_vec4_alloc ();
  const float v[4] = { 1.f, 2.f, 3.f, 4.f };
  float v_out[4];

  graphene_vec4_init (vec4, 0.5f, 0.5f, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.5f);

  graphene_vec4_init (vec4, 1.0f, 1.0f, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, graphene_vec4_get_y (vec4));
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, graphene_vec4_get_z (vec4));
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, graphene_vec4_get_w (vec4));

  graphene_vec4_init_from_vec4 (vec4, graphene_vec4_one ());
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 1.f);

  graphene_vec4_init_from_vec3 (vec4, graphene_vec3_x_axis (), 0.f);
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);

  graphene_vec4_init_from_vec2 (vec4, graphene_vec2_y_axis (), 0.f, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, 0.f);

  graphene_vec4_init_from_float (vec4, v);
  g_assert_cmpfloat (graphene_vec4_get_x (vec4), ==, v[0]);
  g_assert_cmpfloat (graphene_vec4_get_y (vec4), ==, v[1]);
  g_assert_cmpfloat (graphene_vec4_get_z (vec4), ==, v[2]);
  g_assert_cmpfloat (graphene_vec4_get_w (vec4), ==, v[3]);

  graphene_vec4_to_float (vec4, v_out);
  g_assert_cmpfloat (v_out[0], ==, v[0]);
  g_assert_cmpfloat (v_out[1], ==, v[1]);
  g_assert_cmpfloat (v_out[2], ==, v[2]);
  g_assert_cmpfloat (v_out[3], ==, v[3]);

  graphene_vec4_free (vec4);
}

static void
vectors_vec4_ops_add (void)
{
  graphene_vec4_t a, b, res;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);
  graphene_vec4_init (&b, 3.f, 4.f, 5.f, 6.f);

  graphene_vec4_add (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==,  4.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==,  6.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==,  8.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 10.f);
}

static void
vectors_vec4_ops_sub (void)
{
  graphene_vec4_t a, b, res;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);
  graphene_vec4_init (&b, 3.f, 4.f, 5.f, 6.f);

  graphene_vec4_subtract (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, -2.f);
}

static void
vectors_vec4_ops_mul (void)
{
  graphene_vec4_t a, b, res;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);
  graphene_vec4_init (&b, 3.f, 4.f, 5.f, 6.f);

  graphene_vec4_multiply (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==,  3.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==,  8.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 15.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 24.f);
}

static void
vectors_vec4_ops_div (void)
{
  graphene_vec4_t a, b, res;

  graphene_vec4_init (&a, 6.f, 4.f, 2.f, 12.f);
  graphene_vec4_init (&b, 3.f, 2.f, 1.f,  6.f);

  graphene_vec4_divide (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 2.f);
}

static void
vectors_vec4_ops_dot (void)
{
  graphene_vec4_t a, b;
  float dot, check;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);
  graphene_vec4_init (&b, 2.f, 3.f, 4.f, 5.f);

  dot = graphene_vec4_dot (&a, &b);
  check = 1.f * 2.f + 2.f * 3.f + 3.f * 4.f + 4.f * 5.f;
  graphene_assert_fuzzy_equals (dot, check, 0.0001f);
}

static void
vectors_vec4_length (void)
{
  graphene_vec4_t a;
  float len, check;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);

  len = graphene_vec4_length (&a);
  check = sqrtf ((1.f * 1.f) + (2.f * 2.f) + (3.f * 3.f) + (4.f * 4.f));
  graphene_assert_fuzzy_equals (len, check, 0.0001f);
}

static void
vectors_vec4_normalize (void)
{
  graphene_vec4_t a, b;
  float len;

  graphene_vec4_init (&a, 1.f, 2.f, 3.f, 4.f);
  len = graphene_vec4_length (&a);

  graphene_vec4_normalize (&a, &b);

  graphene_assert_fuzzy_equals (graphene_vec4_get_x (&b), graphene_vec4_get_x (&a) / len, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_y (&b), graphene_vec4_get_y (&a) / len, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_z (&b), graphene_vec4_get_z (&a) / len, 0.0001f);
  graphene_assert_fuzzy_equals (graphene_vec4_get_w (&b), graphene_vec4_get_w (&a) / len, 0.0001f);
}

static void
vectors_vec4_compare (void)
{
  const graphene_vec4_t *zero = graphene_vec4_zero ();
  const graphene_vec4_t *one = graphene_vec4_one ();
  graphene_vec4_t a, b;
  graphene_vec4_t res;

  graphene_vec4_min (zero, one, &res);
  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 0.f);

  graphene_vec4_max (zero, one, &res);
  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 1.f);

  graphene_vec4_init (&a, 5.f, 10.f, 8.f, -1.f);
  graphene_vec4_init (&b, 2.f, 12.f, 8.f,  1.f);

  graphene_vec4_min (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, 10.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 8.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, -1.f);

  graphene_vec4_max (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec4_get_x (&res), ==, 5.f);
  g_assert_cmpfloat (graphene_vec4_get_y (&res), ==, 12.f);
  g_assert_cmpfloat (graphene_vec4_get_z (&res), ==, 8.f);
  g_assert_cmpfloat (graphene_vec4_get_w (&res), ==, 1.f);
}

static void
vectors_vec4_conversion (void)
{
  graphene_vec4_t v;

  graphene_vec4_init (&v, 1.f, 2.f, 3.f, 4.f);

  {
    graphene_vec2_t xy;

    graphene_vec4_get_xy (&v, &xy);
    g_assert_cmpfloat (graphene_vec2_get_x (&xy), ==, 1.f);
    g_assert_cmpfloat (graphene_vec2_get_y (&xy), ==, 2.f);
  }

  {
    graphene_vec3_t xyz;

    graphene_vec4_get_xyz (&v, &xyz);
    g_assert_cmpfloat (graphene_vec3_get_x (&xyz), ==, graphene_vec4_get_x (&v));
    g_assert_cmpfloat (graphene_vec3_get_y (&xyz), ==, graphene_vec4_get_y (&v));
    g_assert_cmpfloat (graphene_vec3_get_z (&xyz), ==, graphene_vec4_get_z (&v));
  }
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec4/const", vectors_vec4_const);
  g_test_add_func ("/vectors/vec4/init", vectors_vec4_init);
  g_test_add_func ("/vectors/vec4/operations/add", vectors_vec4_ops_add);
  g_test_add_func ("/vectors/vec4/operations/sub", vectors_vec4_ops_sub);
  g_test_add_func ("/vectors/vec4/operations/mul", vectors_vec4_ops_mul);
  g_test_add_func ("/vectors/vec4/operations/div", vectors_vec4_ops_div);
  g_test_add_func ("/vectors/vec4/operations/dot", vectors_vec4_ops_dot);
  g_test_add_func ("/vectors/vec4/length", vectors_vec4_length);
  g_test_add_func ("/vectors/vec4/normalize", vectors_vec4_normalize);
  g_test_add_func ("/vectors/vec4/compare", vectors_vec4_compare);
  g_test_add_func ("/vectors/vec4/conversion", vectors_vec4_conversion);

  return g_test_run ();
}
