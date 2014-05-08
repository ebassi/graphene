#include <glib.h>
#include <graphene.h>
#include "graphene-bench-utils.h"

typedef struct
{
  graphene_vec4_t *vectors;
  int n_vectors;
} Vec4Bench;

static gpointer
vec4_setup (void)
{
  Vec4Bench *res = g_new0 (Vec4Bench, 1);

  res->n_vectors = 6;
  res->vectors = g_new0 (graphene_vec4_t, res->n_vectors);

  graphene_vec4_init_from_vec4 (&(res->vectors[0]), graphene_vec4_zero ());
  graphene_vec4_init_from_vec4 (&(res->vectors[1]), graphene_vec4_one ());
  graphene_vec4_init_from_vec4 (&(res->vectors[2]), graphene_vec4_x_axis ());
  graphene_vec4_init_from_vec4 (&(res->vectors[3]), graphene_vec4_y_axis ());
  graphene_vec4_init_from_vec4 (&(res->vectors[4]), graphene_vec4_z_axis ());
  graphene_vec4_init_from_vec4 (&(res->vectors[5]), graphene_vec4_w_axis ());

  return res;
}

static void
vec4_dot (gpointer data_)
{
  Vec4Bench *data = data_;
  int n_checks = graphene_bench_get_rounds_per_unit ();
  int i;

  for (i = 0; i < n_checks; i++)
    {
      int a = g_random_int_range (0, data->n_vectors - 1);
      int b = g_random_int_range (0, data->n_vectors - 1);
      graphene_vec4_t *op1 = &(data->vectors[a]);
      graphene_vec4_t *op2 = &(data->vectors[b]);
      float res;

      res = graphene_vec4_dot (op1, op2);
    }
}

static void
vec4_normalize (gpointer data_)
{
  Vec4Bench *data = data_;
  int n_checks = graphene_bench_get_rounds_per_unit ();
  int i;

  for (i = 0; i < n_checks; i++)
    {
      int a = g_random_int_range (0, data->n_vectors - 1);
      graphene_vec4_t *op = &(data->vectors[a]);
      graphene_vec4_t res;

      graphene_vec4_normalize (op, &res);
    }
}

static void
vec4_teardown (gpointer data_)
{
  Vec4Bench *data = data_;

  g_free (data->vectors);
  g_free (data);
}

int
main (int argc, char *argv[])
{
  graphene_bench_init (&argc, &argv,
                       GRAPHENE_BENCH_OPT_IMPLEMENTATION, GRAPHENE_BENCH,
                       NULL);

  graphene_bench_set_fixture_setup (vec4_setup);
  graphene_bench_set_fixture_teardown (vec4_teardown);
  graphene_bench_set_rounds_per_unit (10000);

  graphene_bench_add_func ("/vec4/dot", vec4_dot);
  graphene_bench_add_func ("/vec4/normalize", vec4_normalize);

  return graphene_bench_run ();
}
