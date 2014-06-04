#define _XOPEN_SOURCE 600 /* posix_memalign() */
#include <stdlib.h>
#include <errno.h>
#include <glib.h>
#include "graphene-simd4x4f.h"
#include "graphene-bench-utils.h"

#define N_ROUNDS 819200

typedef struct
{
  graphene_simd4x4f_t *a;
  graphene_simd4x4f_t *b;
  graphene_simd4x4f_t *c;
} MatrixBench;

static gpointer
alloc_n_matrix (gsize n)
{
  gpointer res;

  if (posix_memalign (&res, 16, n * sizeof (graphene_simd4x4f_t)) != 0)
    {
      int saved_errno = errno;

      g_error ("posix_memalign: %s\n", g_strerror (saved_errno));
    }

  return res;
}

static gpointer
matrix_setup (void)
{
  MatrixBench *res = g_new0 (MatrixBench, 1);
  gpointer mem;
  int i;

  res->a = alloc_n_matrix (N_ROUNDS);
  res->b = alloc_n_matrix (N_ROUNDS);
  res->c = alloc_n_matrix (N_ROUNDS);

  for (i = 0; i < N_ROUNDS; i++)
    {
      graphene_simd4f_t p, q;

      p = graphene_simd4f_init (i, i, i, i);
      q = graphene_simd4f_init (N_ROUNDS - i, N_ROUNDS - i, N_ROUNDS - i, N_ROUNDS - i);

      res->a[i] = graphene_simd4x4f_init (p, p, p, p);
      res->b[i] = graphene_simd4x4f_init (q, q, q, q);
    }

  return res;
}

static void
matrix_multiply (gpointer data_)
{
  MatrixBench *data = data_;
  int i;

  for (i = 0; i < N_ROUNDS; i++)
    graphene_simd4x4f_matrix_mul (&(data->a[i]), &(data->b[i]), &(data->c[i]));
}

static void
matrix_teardown (gpointer data_)
{
  MatrixBench *data = data_;

  g_free (data->a);
  g_free (data->b);
  g_free (data->c);
  g_free (data);
}

int
main (int argc, char *argv[])
{
  graphene_bench_init (&argc, &argv, NULL);

  graphene_bench_set_fixture_setup (matrix_setup);
  graphene_bench_set_fixture_teardown (matrix_teardown);
  graphene_bench_set_rounds_per_unit (N_ROUNDS);

  graphene_bench_add_func ("/simd/4x4f/multiply", matrix_multiply);

  return graphene_bench_run ();
}
