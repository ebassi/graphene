#include <glib.h>
#include <graphene.h>

static void
matrix_identity (void)
{
  graphene_matrix_t m;
  float v[16];

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_to_float (&m, v);
  g_assert_cmpfloat (v[0],  ==, 1.0f);
  g_assert_cmpfloat (v[5],  ==, 1.0f);
  g_assert_cmpfloat (v[10], ==, 1.0f);

  graphene_matrix_scale (&m, 2.0f, 2.0f, 2.0f);
  g_assert_false (graphene_matrix_is_identity (&m));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/matrix/identity", matrix_identity);

  return g_test_run ();
}
