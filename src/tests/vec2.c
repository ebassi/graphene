#include <graphene.h>

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
  graphene_vec2_t vec2;

  graphene_vec2_init (&vec2, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec2_get_x (&vec2), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec2_get_y (&vec2), ==, 0.5f);

  graphene_vec2_init (&vec2, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec2_get_x (&vec2), ==, graphene_vec2_get_y (&vec2));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec2/const", vectors_vec2_const);
  g_test_add_func ("/vectors/vec2/init", vectors_vec2_init);

  return g_test_run ();
}
