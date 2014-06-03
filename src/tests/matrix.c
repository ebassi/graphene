#include <glib.h>
#include <graphene.h>
#include <math.h>

static float
deg_to_rad (float degree)
{
  return degree * GRAPHENE_PI / 180.0f;
}

static void
g_assert_cmpfloat_delta (float n1,
                         float n2,
                         float delta)
{
  g_assert (fabsf (n1 - n2) < delta);
}

static void
compare_matrices (const graphene_matrix_t * m1,
                  const graphene_matrix_t * m2)
{
  int x, y;
  for (x = 0; x < 4; x++)
  {
    for (y = 0; y < 4; y++)
    {
      g_assert_cmpfloat_delta (graphene_matrix_get_value (m1, x, y),
                               graphene_matrix_get_value (m2, x, y), 0.1);
    }
  }
}

static void
matrix_identity (void)
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
}

static void
matrix_rotation (void)
{
  graphene_matrix_t m;
  graphene_matrix_t m2;

  graphene_matrix_init_identity (&m);
  g_assert_true (graphene_matrix_is_identity (&m));

  graphene_matrix_rotate (&m, deg_to_rad(180.0f), graphene_vec3_x_axis());
  graphene_matrix_init_rotate (&m2, deg_to_rad(180.0f), graphene_vec3_x_axis());

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);
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

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);
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

  if (g_test_verbose ())
    graphene_matrix_print (&m);

  compare_matrices (&m, &m2);
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
