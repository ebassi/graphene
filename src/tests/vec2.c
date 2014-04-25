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
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec2/const", vectors_vec2_const);

  return g_test_run ();
}
