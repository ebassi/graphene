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

  graphene_simd4f_t *pa;
  graphene_simd4f_t *qa;
  graphene_simd4f_t *ra;
} MatrixBench;

static gpointer
alloc_align (gsize n, gsize type_size)
{
  gpointer res;

  if (posix_memalign (&res, 16, n * type_size) != 0)
    {
      int saved_errno = errno;

      g_error ("posix_memalign: %s\n", g_strerror (saved_errno));
    }

  return res;
}

#define alloc_n_matrix(n)       alloc_align((n), sizeof (graphene_simd4x4f_t))
#define alloc_n_vec(n)          alloc_align((n), sizeof (graphene_simd4f_t))

static gpointer
matrix_setup (void)
{
  MatrixBench *res = g_new0 (MatrixBench, 1);
  gpointer mem;
  int i;

  res->a = alloc_n_matrix (N_ROUNDS);
  res->b = alloc_n_matrix (N_ROUNDS);
  res->c = alloc_n_matrix (N_ROUNDS);

  res->pa = alloc_n_vec (N_ROUNDS);
  res->qa = alloc_n_vec (N_ROUNDS);
  res->ra = alloc_n_vec (N_ROUNDS);

  for (i = 0; i < N_ROUNDS; i++)
    {
      graphene_simd4f_t p, q;

      p = graphene_simd4f_init (i, i, i, i);
      q = graphene_simd4f_init (N_ROUNDS - i, N_ROUNDS - i, N_ROUNDS - i, N_ROUNDS - i);

      res->a[i] = graphene_simd4x4f_init (p, p, p, p);
      res->b[i] = graphene_simd4x4f_init (q, q, q, q);

      res->pa[i] = graphene_simd4f_init (i, i, 0.f, 0.f);
      res->qa[i] = graphene_simd4f_init (N_ROUNDS - i, N_ROUNDS - 1, 1.f, 0.f);
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
matrix_project (gpointer data_)
{
  MatrixBench *data = data_;
  int i;

  for (i = 0; i < N_ROUNDS; i++)
    {
      graphene_simd4f_t pback, qback, uback;
      float t, x, y;

      graphene_simd4x4f_vec3_mul (&(data->a[i]), &(data->pa[i]), &pback);
      graphene_simd4x4f_vec3_mul (&(data->a[i]), &(data->qa[i]), &qback);

      uback = graphene_simd4f_sub (data->pa[i], pback);
      t = -1.0f * graphene_simd4f_get_z (pback) / graphene_simd4f_get_z (uback);
      x = graphene_simd4f_get_x (pback) + t * graphene_simd4f_get_x (uback);
      y = graphene_simd4f_get_y (pback) + t * graphene_simd4f_get_y (uback);

      data->ra[i] = graphene_simd4f_init (x, y, 0.f, 0.f);
    }
}

static void
matrix_teardown (gpointer data_)
{
  MatrixBench *data = data_;

  g_free (data->a);
  g_free (data->b);
  g_free (data->c);
  g_free (data->pa);
  g_free (data->qa);
  g_free (data->ra);
  g_free (data);
}

int
main (int argc, char *argv[])
{
  graphene_bench_init (&argc, &argv,
                       "implementation", IMPL,
                       NULL);

  graphene_bench_set_fixture_setup (matrix_setup);
  graphene_bench_set_fixture_teardown (matrix_teardown);
  graphene_bench_set_rounds_per_unit (N_ROUNDS);

  graphene_bench_add_func ("/simd/4x4f/multiply", matrix_multiply);
  graphene_bench_add_func ("/simd/4x4f/project", matrix_project);

  return graphene_bench_run ();
}
