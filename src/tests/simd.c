#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
simd_compare_eq (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (1.f, 0.f, 1.f, 0.f);
  g_assert_true (graphene_simd4f_cmp_eq (a, a));

  b = graphene_simd4f_init (1.f, 0.f, 1.f, 0.f);
  g_assert_true (graphene_simd4f_cmp_eq (a, b));
  g_assert_true (graphene_simd4f_cmp_eq (b, a));

  c = graphene_simd4f_splat (1.f);
  g_assert_false (graphene_simd4f_cmp_eq (a, c));
  g_assert_false (graphene_simd4f_cmp_eq (b, c));
}

static void
simd_compare_neq (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (1.f, 0.f, 1.f, 0.f);
  g_assert_false (graphene_simd4f_cmp_neq (a, a));

  b = graphene_simd4f_init (1.f, 0.f, 1.f, 0.f);
  g_assert_false (graphene_simd4f_cmp_neq (a, b));
  g_assert_false (graphene_simd4f_cmp_neq (b, a));

  c = graphene_simd4f_splat (1.f);
  g_assert_true (graphene_simd4f_cmp_neq (a, c));
  g_assert_true (graphene_simd4f_cmp_neq (b, c));
}

static void
simd_compare_lt (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  b = graphene_simd4f_init (1.f, 1.f, 1.f, 1.f);
  g_assert_true (graphene_simd4f_cmp_lt (a, b));
  g_assert_false (graphene_simd4f_cmp_lt (b, a));

  c = graphene_simd4f_init (0.f, 1.f, 0.f, 1.f);
  g_assert_false (graphene_simd4f_cmp_lt (a, c));
}

static void
simd_compare_le (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  b = graphene_simd4f_init (1.f, 1.f, 1.f, 1.f);
  g_assert_true (graphene_simd4f_cmp_le (a, b));
  g_assert_false (graphene_simd4f_cmp_le (b, a));

  c = graphene_simd4f_init (0.f, 1.f, 0.f, 1.f);
  g_assert_true (graphene_simd4f_cmp_le (a, c));
  g_assert_false (graphene_simd4f_cmp_le (b, c));
}

static void
simd_compare_ge (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  b = graphene_simd4f_init (1.f, 1.f, 1.f, 1.f);
  g_assert_false (graphene_simd4f_cmp_ge (a, b));
  g_assert_true (graphene_simd4f_cmp_ge (b, a));

  c = graphene_simd4f_init (0.f, 1.f, 0.f, 1.f);
  g_assert_false (graphene_simd4f_cmp_ge (a, c));
  g_assert_true (graphene_simd4f_cmp_ge (b, c));
}

static void
simd_compare_gt (void)
{
  graphene_simd4f_t a, b, c;

  a = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  b = graphene_simd4f_init (1.f, 1.f, 1.f, 1.f);
  g_assert_false (graphene_simd4f_cmp_gt (a, b));
  g_assert_true (graphene_simd4f_cmp_gt (b, a));

  c = graphene_simd4f_init (0.f, 1.f, 0.f, 1.f);
  g_assert_false (graphene_simd4f_cmp_gt (c, a));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/simd/compare/eq", simd_compare_eq);
  g_test_add_func ("/simd/compare/neq", simd_compare_neq);
  g_test_add_func ("/simd/compare/lt", simd_compare_lt);
  g_test_add_func ("/simd/compare/le", simd_compare_le);
  g_test_add_func ("/simd/compare/ge", simd_compare_ge);
  g_test_add_func ("/simd/compare/gt", simd_compare_gt);

  return g_test_run ();
}
