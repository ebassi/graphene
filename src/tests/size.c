#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
size_zero (void)
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

static void
size_equal (void)
{
  const graphene_size_t *zero;

  zero = graphene_size_zero ();

  g_assert_true (graphene_size_equal (zero, zero));
  g_assert_false (graphene_size_equal (zero, NULL));
  g_assert_false (graphene_size_equal (NULL, zero));
}

static void
size_init (void)
{
  graphene_size_t s;

  graphene_size_init (&s, 10.f, 20.f);
  g_assert_cmpfloat (s.width, ==, 10.f);
  g_assert_cmpfloat (s.height, ==, 20.f);
}

static void
size_scale (void)
{
  graphene_size_t s, r;

  graphene_size_init (&s, 10.f, 10.f);
  graphene_size_scale (&s, .5f, &r);
  g_assert_cmpfloat (s.width, >, r.width);
  g_assert_cmpfloat (s.height, >, r.height);
  g_assert_cmpfloat (s.width, ==, (r.width * 2.f));
  g_assert_cmpfloat (s.height, ==, (r.height * 2.f));
}

static void
size_interpolate (void)
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

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/size/init", size_init);
  g_test_add_func ("/size/zero", size_zero);
  g_test_add_func ("/size/equal", size_equal);
  g_test_add_func ("/size/scale", size_scale);
  g_test_add_func ("/size/interpolate", size_interpolate);

  return g_test_run ();
}
