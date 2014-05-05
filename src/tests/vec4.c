#include <graphene.h>

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
  graphene_vec4_t vec4;

  graphene_vec4_init (&vec4, 0.5f, 0.5f, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_x (&vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_y (&vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_z (&vec4), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec4_get_w (&vec4), ==, 0.5f);

  graphene_vec4_init (&vec4, 1.0f, 1.0f, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec4_get_x (&vec4), ==, graphene_vec4_get_y (&vec4));
  g_assert_cmpfloat (graphene_vec4_get_x (&vec4), ==, graphene_vec4_get_z (&vec4));
  g_assert_cmpfloat (graphene_vec4_get_x (&vec4), ==, graphene_vec4_get_w (&vec4));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec4/const", vectors_vec4_const);
  g_test_add_func ("/vectors/vec4/init", vectors_vec4_init);

  return g_test_run ();
}
