#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (size_zero)
{
  const graphene_size_t *zero;
  graphene_size_t *s;

  zero = graphene_size_zero ();
  g_assert_cmpfloat (zero->width, ==, 0.f);
  g_assert_cmpfloat (zero->height, ==, 0.f);

  g_assert_true (zero == graphene_size_zero ());
  g_assert_true (graphene_size_equal (zero, graphene_size_zero ()));

  s = graphene_size_init_from_size (graphene_size_alloc (), graphene_size_zero ());
  g_assert_false (s == zero);
  g_assert_true (graphene_size_equal (s, zero));

  graphene_size_free (s);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (size_equal)
{
  const graphene_size_t *zero;

  zero = graphene_size_zero ();

  g_assert_true (graphene_size_equal (zero, zero));
  g_assert_false (graphene_size_equal (zero, NULL));
  g_assert_false (graphene_size_equal (NULL, zero));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (size_init)
{
  graphene_size_t s;

  graphene_size_init (&s, 10.f, 20.f);
  g_assert_cmpfloat (s.width, ==, 10.f);
  g_assert_cmpfloat (s.height, ==, 20.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (size_scale)
{
  graphene_size_t s, r;

  graphene_size_init (&s, 10.f, 10.f);
  graphene_size_scale (&s, .5f, &r);
  g_assert_cmpfloat (s.width, >, r.width);
  g_assert_cmpfloat (s.height, >, r.height);
  g_assert_cmpfloat (s.width, ==, (r.width * 2.f));
  g_assert_cmpfloat (s.height, ==, (r.height * 2.f));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (size_interpolate)
{
  graphene_size_t a, b, half, res;

  graphene_size_init (&a, 10.f, 10.f);
  graphene_size_init (&b, 20.f, 20.f);
  graphene_size_init (&half, 15.f, 15.f);

  graphene_size_interpolate (&a, &b, 0.0, &res);
  g_assert_true (graphene_size_equal (&a, &res));

  graphene_size_interpolate (&a, &b, 1.0, &res);
  g_assert_true (graphene_size_equal (&b, &res));

  graphene_size_interpolate (&a, &b, 0.5, &res);
  g_assert_true (graphene_size_equal (&half, &res));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/size/init", size_init)
  GRAPHENE_TEST_UNIT ("/size/zero", size_zero)
  GRAPHENE_TEST_UNIT ("/size/equal", size_equal)
  GRAPHENE_TEST_UNIT ("/size/scale", size_scale)
  GRAPHENE_TEST_UNIT ("/size/interpolate", size_interpolate)
)
