#include <glib.h>
#include <graphene.h>
#include "graphene-bench-utils.h"

typedef struct
{
  graphene_vec2_t *vectors;
  int n_vectors;
} Vec2Bench;

static gpointer
vec2_setup (void)
{
  Vec2Bench *res = g_new0 (Vec2Bench, 1);

  res->n_vectors = 4;
  res->vectors = g_new0 (graphene_vec2_t, res->n_vectors);

  graphene_vec2_init_from_vec2 (&(res->vectors[0]), graphene_vec2_zero ());
  graphene_vec2_init_from_vec2 (&(res->vectors[1]), graphene_vec2_one ());
  graphene_vec2_init_from_vec2 (&(res->vectors[2]), graphene_vec2_x_axis ());
  graphene_vec2_init_from_vec2 (&(res->vectors[3]), graphene_vec2_y_axis ());

  return res;
}

static void
vec2_dot (gpointer data_)
{
  Vec2Bench *data = data_;
  int n_checks = graphene_bench_get_rounds_per_unit ();
  int i;

  for (i = 0; i < n_checks; i++)
    {
      int a = g_random_int_range (0, 3);
      int b = g_random_int_range (0, 3);
      graphene_vec2_t *op1 = &(data->vectors[a]);
      graphene_vec2_t *op2 = &(data->vectors[b]);
      float res;

      res = graphene_vec2_dot (op1, op2);
    }
}

static void
vec2_teardown (gpointer data_)
{
  Vec2Bench *data = data_;

  g_free (data->vectors);
  g_free (data);
}

int
main (int argc, char *argv[])
{
  graphene_bench_init (&argc, &argv,
                       GRAPHENE_BENCH_OPT_IMPLEMENTATION, GRAPHENE_BENCH,
                       NULL);

  graphene_bench_set_fixture_setup (vec2_setup);
  graphene_bench_set_fixture_teardown (vec2_teardown);
  graphene_bench_set_rounds_per_unit (10000);

  graphene_bench_add_func ("/vec2/dot", vec2_dot);

  return graphene_bench_run ();
}
