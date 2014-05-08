#include <glib.h>
#include <graphene.h>
#include "graphene-bench-utils.h"

typedef struct
{
  graphene_matrix_t start;
  graphene_matrix_t end;
} MatrixBench;

static gpointer
matrix_setup (void)
{
  MatrixBench *res = g_new0 (MatrixBench, 1);
  graphene_point3d_t t_start = GRAPHENE_POINT3D_INIT (24.f, 24.f, 0.f);
  graphene_point3d_t t_end = GRAPHENE_POINT3D_INIT (0.f, 0.f, 0.f);

  graphene_matrix_init_identity (&res->start);
  graphene_matrix_translate (&res->start, &t_start);
  graphene_matrix_scale (&res->start, 1.f, 1.f, 1.f);

  graphene_matrix_init_identity (&res->end);
  graphene_matrix_translate (&res->end, &t_end);
  graphene_matrix_scale (&res->end, 2.f, 2.f, 1.f);
  graphene_matrix_rotate (&res->end, 360.f * (G_PI / 180.f), graphene_vec3_z_axis ());

  return res;
}

static void
matrix_interpolate (gpointer data_)
{
  MatrixBench *data = data_;
  int n_steps = graphene_bench_get_rounds_per_unit ();
  int i;

  for (i = 0; i < n_steps; i++)
    {
      double progress = (double) i / n_steps;
      graphene_matrix_t res;

      graphene_matrix_interpolate (&data->start, &data->end, progress, &res);
    }
}

static void
matrix_teardown (gpointer data_)
{
  MatrixBench *data = data_;

  g_free (data);
}

int
main (int argc, char *argv[])
{
  graphene_bench_init (&argc, &argv,
                       GRAPHENE_BENCH_OPT_IMPLEMENTATION, GRAPHENE_BENCH,
                       NULL);

  graphene_bench_set_fixture_setup (matrix_setup);
  graphene_bench_set_fixture_teardown (matrix_teardown);
  graphene_bench_set_rounds_per_unit (1000);

  graphene_bench_add_func ("/matrix/interpolate", matrix_interpolate);

  return graphene_bench_run ();
}
