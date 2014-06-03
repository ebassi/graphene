#include <glib.h>
#include <graphene.h>

static void
print_matrix (graphene_matrix_t * m)
{
  int i;
  g_print("\n");
  for (i = 0; i < 4; i++) {
    g_print ("%.3f %.3f %.3f %.3f\n",
      graphene_matrix_get_value (m, i, 0),
      graphene_matrix_get_value (m, i, 1),
      graphene_matrix_get_value (m, i, 2),
      graphene_matrix_get_value (m, i, 3));
  }
}

static void
compare_matrices (graphene_matrix_t * m1, graphene_matrix_t * m2)
{
  int x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      g_assert_cmpfloat (
        graphene_matrix_get_value (m1, x, y), ==,
        graphene_matrix_get_value (m2, x, y));
}

static void
matrix_identity (void)
{
  graphene_matrix_t m;
  float v[16];

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));
  print_matrix(&m);

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

  graphene_vec3_t rot;
  graphene_vec3_init (&rot, 1.0, 0.0, 0.0);

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_rotate (&m, 30.0f, &rot);
  graphene_matrix_init_rotate (&m2, 30.0f, &rot);
  print_matrix(&m);

  compare_matrices(&m, &m2);
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
  print_matrix(&m);

  compare_matrices(&m, &m2);
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
  print_matrix(&m);

  compare_matrices(&m, &m2);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/matrix/identity", matrix_identity);
  g_test_add_func ("/matrix/scale", matrix_scale);
  g_test_add_func ("/matrix/rotation", matrix_rotation);
  g_test_add_func ("/matrix/translation", matrix_translation);

  return g_test_run ();
}
