#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
vectors_vec2_const (void)
{
  const graphene_vec2_t *vec2;

  vec2 = graphene_vec2_zero ();
  g_assert_nonnull (vec2);
  g_assert_true (vec2 == graphene_vec2_zero ());
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 0.f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 0.f);
  g_assert_cmpfloat (graphene_vec2_length (vec2), ==, 0.f);

  vec2 = graphene_vec2_one ();
  g_assert_nonnull (vec2);
  g_assert_true (vec2 == graphene_vec2_one ());
  g_assert_true (vec2 != graphene_vec2_zero ());
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 1.f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 1.f);

  vec2 = graphene_vec2_x_axis ();
  g_assert_nonnull (vec2);
  g_assert_true (vec2 == graphene_vec2_x_axis ());
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 1.f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 0.f);

  vec2 = graphene_vec2_y_axis ();
  g_assert_nonnull (vec2);
  g_assert_true (vec2 == graphene_vec2_y_axis ());
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 0.f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 1.f);
}

static void
vectors_vec2_init (void)
{
  graphene_vec2_t *vec2 = graphene_vec2_alloc ();
  const float v[2] = { 3.f, 4.f };
  float v_out[2];

  graphene_vec2_init (vec2, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 0.5f);

  graphene_vec2_init (vec2, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, graphene_vec2_get_y (vec2));

  graphene_vec2_init_from_vec2 (vec2, graphene_vec2_one ());
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, 1.f);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, 1.f);

  graphene_vec2_init_from_float (vec2, v);
  g_assert_cmpfloat (graphene_vec2_get_x (vec2), ==, v[0]);
  g_assert_cmpfloat (graphene_vec2_get_y (vec2), ==, v[1]);

  graphene_vec2_to_float (vec2, v_out);
  g_assert_cmpfloat (v_out[0], ==, v[0]);
  g_assert_cmpfloat (v_out[1], ==, v[1]);

  graphene_vec2_free (vec2);
}

static void
vectors_vec2_ops_add (void)
{
  graphene_vec2_t a, b, res;

  graphene_vec2_init (&a, 1.f, 2.f);
  graphene_vec2_init (&b, 3.f, 4.f);

  graphene_vec2_add (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 4.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 6.f);
}

static void
vectors_vec2_ops_sub (void)
{
  graphene_vec2_t a, b, res;

  graphene_vec2_init (&a, 1.f, 2.f);
  graphene_vec2_init (&b, 3.f, 4.f);

  graphene_vec2_subtract (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, -2.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, -2.f);
}

static void
vectors_vec2_ops_mul (void)
{
  graphene_vec2_t a, b, res;

  graphene_vec2_init (&a, 1.f, 2.f);
  graphene_vec2_init (&b, 3.f, 4.f);

  graphene_vec2_multiply (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 3.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 8.f);
}

static void
vectors_vec2_ops_div (void)
{
  graphene_vec2_t a, b, res;

  graphene_vec2_init (&a, 6.f, 4.f);
  graphene_vec2_init (&b, 3.f, 2.f);

  graphene_vec2_divide (&a, &b, &res);

  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 2.f);
}

static void
vectors_vec2_ops_dot (void)
{
  graphene_vec2_t a, b;
  float dot, check;

  graphene_vec2_init (&a, 1.f, 2.f);
  graphene_vec2_init (&b, 2.f, 3.f);

  dot = graphene_vec2_dot (&a, &b);
  check = 1.f * 2.f + 2.f * 3.f;
  graphene_assert_fuzzy_equals (dot, check, 0.0001);
}

static void
vectors_vec2_length (void)
{
  graphene_vec2_t a;
  float len, check;

  graphene_vec2_init (&a, 10.f, 20.f);

  len = graphene_vec2_length (&a);
  check = sqrtf ((10.f * 10.f) + (20.f * 20.f));
  graphene_assert_fuzzy_equals (len, check, 0.0001);
}

static void
vectors_vec2_normalize (void)
{
  graphene_vec2_t a, b;
  float len;

  graphene_vec2_init (&a, 10.f, 20.f);
  len = graphene_vec2_length (&a);

  graphene_vec2_normalize (&a, &b);

  graphene_assert_fuzzy_equals (graphene_vec2_get_x (&b),
                                (graphene_vec2_get_x (&a) / len),
                                0.0001);
  graphene_assert_fuzzy_equals (graphene_vec2_get_y (&b),
                                (graphene_vec2_get_y (&a) / len),
                                0.0001);
}

static void
vectors_vec2_compare (void)
{
  const graphene_vec2_t *zero = graphene_vec2_zero ();
  const graphene_vec2_t *one = graphene_vec2_one ();
  graphene_vec2_t a, b;
  graphene_vec2_t res;

  graphene_vec2_min (zero, one, &res);
  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 0.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 0.f);

  graphene_vec2_max (zero, one, &res);
  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 1.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 1.f);

  graphene_vec2_init (&a, 5.f, 10.f);
  graphene_vec2_init (&b, 2.f, 12.f);

  graphene_vec2_min (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 2.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 10.f);

  graphene_vec2_max (&a, &b, &res);
  g_assert_cmpfloat (graphene_vec2_get_x (&res), ==, 5.f);
  g_assert_cmpfloat (graphene_vec2_get_y (&res), ==, 12.f);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec2/const", vectors_vec2_const);
  g_test_add_func ("/vectors/vec2/init", vectors_vec2_init);
  g_test_add_func ("/vectors/vec2/operations/add", vectors_vec2_ops_add);
  g_test_add_func ("/vectors/vec2/operations/sub", vectors_vec2_ops_sub);
  g_test_add_func ("/vectors/vec2/operations/mul", vectors_vec2_ops_mul);
  g_test_add_func ("/vectors/vec2/operations/div", vectors_vec2_ops_div);
  g_test_add_func ("/vectors/vec2/operations/dot", vectors_vec2_ops_dot);
  g_test_add_func ("/vectors/vec2/length", vectors_vec2_length);
  g_test_add_func ("/vectors/vec2/normalize", vectors_vec2_normalize);
  g_test_add_func ("/vectors/vec2/compare", vectors_vec2_compare);

  return g_test_run ();
}
