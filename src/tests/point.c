#include <math.h>
#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
point_zero (void)
{
  const graphene_point_t *p;
  graphene_point_t q;

  p = graphene_point_zero ();
  g_assert_nonnull (p);
  g_assert_true (p == graphene_point_zero ());
  g_assert_cmpfloat (p->x, ==, 0.f);
  g_assert_cmpfloat (p->y, ==, 0.f);

  graphene_point_init_from_point (&q, graphene_point_zero ());
  g_assert_cmpfloat (q.x, ==, 0.f);
  g_assert_cmpfloat (q.y, ==, 0.f);
  g_assert_cmpfloat (graphene_point_distance (p, &q, NULL, NULL), ==, 0.f);
}

static void
point_alloc (void)
{
  graphene_point_t *a, b;

  a = graphene_point_alloc ();
  g_assert_nonnull (a);

  graphene_point_init (a, 10.f, 10.f);
  graphene_point_init_from_point (&b, a);
  g_assert_true (graphene_point_equal (&b, a));

  graphene_point_free (a);
}

static void
point_init (void)
{
  graphene_point_t p, q;

  graphene_point_init (&p, 0.f, 0.f);
  g_assert_cmpfloat (p.x, ==, 0.f);
  g_assert_cmpfloat (p.y, ==, 0.f);

  graphene_point_init (&p, 1.f, 1.f);
  g_assert_cmpfloat (p.x, ==, 1.0f);
  g_assert_cmpfloat (p.y, ==, 1.0f);

  graphene_point_init_from_point (&q, &p);
  g_assert_cmpfloat (q.x, ==, p.x);
  g_assert_cmpfloat (q.y, ==, p.y);
}

static void
point_equal (void)
{
  graphene_point_t p, q;

  graphene_point_init (&p, 0.f, 0.f);
  graphene_point_init (&q, 0.f, 0.f);

  g_assert_true (graphene_point_equal (&p, &p));
  g_assert_false (graphene_point_equal (&p, NULL));
  g_assert_false (graphene_point_equal (NULL, &q));
  g_assert_true (graphene_point_equal (&p, &q));

  graphene_point_init (&q, 1.f, 1.f);
  g_assert_false (graphene_point_equal (&p, &q));
}

static void
point_distance (void)
{
  graphene_point_t p, q;
  float x_d, y_d;

  graphene_point_init (&p, 0.f, 0.f);
  g_assert_cmpfloat (p.x, ==, 0.f);
  g_assert_cmpfloat (p.y, ==, 0.f);

  graphene_point_init (&q, 1.f, 1.f);
  g_assert_cmpfloat (q.x, ==, 1.0f);
  g_assert_cmpfloat (q.y, ==, 1.0f);

  g_assert_cmpfloat (graphene_point_distance (&p, &p, NULL, NULL), ==, 0.f);
  g_assert_cmpfloat (graphene_point_distance (&q, &q, NULL, NULL), ==, 0.f);

  x_d = y_d = 0.f;
  graphene_assert_fuzzy_equals (graphene_point_distance (&p, &q, &x_d, &y_d),
                                sqrtf (2.f),
                                0.0001f);
  g_assert_cmpfloat (x_d, ==, 1.f);
  g_assert_cmpfloat (y_d, ==, 1.f);
}

static void
point_near (void)
{
  graphene_point_t p, q;

  graphene_point_init (&p, 0.f, 0.f);
  graphene_point_init (&q, 0.1f, 0.1f);

  g_assert_true (graphene_point_near (&p, &q, 0.2f));
  g_assert_false (graphene_point_near (&p, &q, 0.001f));
  g_assert_false (graphene_point_near (&p, &q, 0.f));
  g_assert_true (graphene_point_near (&p, &p, 0.f));
}

static void
point_interpolate (void)
{
  graphene_point_t p, half, q, res;

  graphene_point_init (&p, 0.f, 0.f);
  graphene_point_init (&q, 1.f, 1.f);
  graphene_point_init (&half, .5f, .5f);

  graphene_point_interpolate (&p, &q, 0.0, &res);
  g_assert_cmpfloat (res.x, ==, p.x);
  g_assert_cmpfloat (res.y, ==, p.y);

  graphene_point_interpolate (&p, &q, 0.5, &res);
  g_assert_cmpfloat (res.x, ==, half.x);
  g_assert_cmpfloat (res.y, ==, half.y);

  graphene_point_interpolate (&p, &q, 1.0, &res);
  g_assert_cmpfloat (res.x, ==, q.x);
  g_assert_cmpfloat (res.y, ==, q.y);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/point/alloc", point_alloc);
  g_test_add_func ("/point/init", point_init);
  g_test_add_func ("/point/equal", point_equal);
  g_test_add_func ("/point/distance", point_distance);
  g_test_add_func ("/point/near", point_near);
  g_test_add_func ("/point/zero", point_zero);
  g_test_add_func ("/point/interpolate", point_interpolate);

  return g_test_run ();
}
