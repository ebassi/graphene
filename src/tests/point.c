#include <math.h>
#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (point_zero)
{
  const graphene_point_t *p;
  graphene_point_t q;

  p = graphene_point_zero ();
  g_assert_nonnull (p);
  g_assert_true (p == graphene_point_zero ());
  g_assert_true (graphene_point_equal (p, &zero2));

  graphene_point_init_from_point (&q, graphene_point_zero ());
  g_assert_true (graphene_point_equal (&q, &zero2));
  g_assert_cmpfloat (graphene_point_distance (p, &q, NULL, NULL), ==, 0.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_alloc)
{
  graphene_point_t *a, b;

  a = graphene_point_alloc ();
  g_assert_nonnull (a);

  graphene_point_init (a, x, y);
  graphene_point_init_from_point (&b, a);
  g_assert_true (graphene_point_equal (&b, a));

  graphene_point_free (a);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_init)
{
  graphene_point_t p, q;

  graphene_point_init (&p, 0.f, 0.f);
  g_assert_cmpfloat (p.x, ==, 0.f);
  g_assert_cmpfloat (p.y, ==, 0.f);

  graphene_point_init (&p, 1.f, 1.f);
  g_assert_cmpfloat (p.x, ==, 1.0f);
  g_assert_cmpfloat (p.y, ==, 1.0f);

  graphene_point_init_from_point (&q, &p);
  graphene_assert_fuzzy_point_equal (&p, &q, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_equal)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_distance)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_near)
{
  graphene_point_t p, q;

  graphene_point_init (&p, 0.f, 0.f);
  graphene_point_init (&q, 0.1f, 0.1f);

  g_assert_true (graphene_point_near (&p, &q, 0.2f));
  g_assert_false (graphene_point_near (&p, &q, 0.001f));
  g_assert_false (graphene_point_near (&p, &q, 0.f));
  g_assert_true (graphene_point_near (&p, &p, 0.f));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_interpolate)
{
  graphene_point_t p, half, q, res;

  graphene_point_init (&p, 0.f, 0.f);
  graphene_point_init (&q, 1.f, 1.f);
  graphene_point_init (&half, .5f, .5f);

  graphene_point_interpolate (&p, &q, 0.0, &res);
  graphene_assert_fuzzy_point_equal (&res, &p, 0.0001);

  graphene_point_interpolate (&p, &q, 0.5, &res);
  graphene_assert_fuzzy_point_equal (&res, &half, 0.0001);

  graphene_point_interpolate (&p, &q, 1.0, &res);
  graphene_assert_fuzzy_point_equal (&res, &q, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/point/alloc", point_alloc)
  GRAPHENE_TEST_UNIT ("/point/init", point_init)
  GRAPHENE_TEST_UNIT ("/point/equal", point_equal)
  GRAPHENE_TEST_UNIT ("/point/distance", point_distance)
  GRAPHENE_TEST_UNIT ("/point/near", point_near)
  GRAPHENE_TEST_UNIT ("/point/zero", point_zero)
  GRAPHENE_TEST_UNIT ("/point/interpolate", point_interpolate)
)
