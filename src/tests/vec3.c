#include <graphene.h>

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
  graphene_vec3_t vec3;

  graphene_vec3_init (&vec3, 0.5f, 0.5f, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_y (&vec3), ==, 0.5f);
  g_assert_cmpfloat (graphene_vec3_get_z (&vec3), ==, 0.5f);

  graphene_vec3_init (&vec3, 1.0f, 1.0f, 1.0f);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, graphene_vec3_get_y (&vec3));
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, graphene_vec3_get_z (&vec3));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/vectors/vec3/const", vectors_vec3_const);
  g_test_add_func ("/vectors/vec3/init", vectors_vec3_init);

  return g_test_run ();
}
