#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
rect_init (void)
{
  graphene_rect_t *r;
  graphene_rect_t s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  g_assert_cmpfloat (r->origin.x, ==, 0.f);
  g_assert_cmpfloat (r->origin.y, ==, 0.f);
  g_assert_cmpfloat (r->size.width, ==, 10.f);
  g_assert_cmpfloat (r->size.height, ==, 10.f);

  graphene_rect_init_from_rect (&s, r);
  g_assert_true (graphene_rect_equal (&s, r));

  graphene_rect_free (r);
}

static void
rect_normalize (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (10.f, 10.f, -10.f, -10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT ( 0.f,  0.f,  10.f,  10.f);

  g_assert_cmpfloat (graphene_rect_get_x (&r), ==, 0.f);
  g_assert_cmpfloat (graphene_rect_get_y (&r), ==, 0.f);
  g_assert_cmpfloat (graphene_rect_get_width (&r), ==, 10.f);
  g_assert_cmpfloat (graphene_rect_get_height (&r), ==, 10.f);

  g_assert_true (graphene_rect_equal (&r, &s));

  graphene_rect_normalize (&r);
  g_assert_true (graphene_rect_equal (&r, &s));
}

static void
rect_equal (void)
{
  graphene_rect_t *r, *s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  s = graphene_rect_init (graphene_rect_alloc (), 1.f, 1.f,  9.f,  9.f);

  g_assert_true (graphene_rect_equal (r, r));
  g_assert_false (graphene_rect_equal (r, NULL));
  g_assert_false (graphene_rect_equal (NULL, r));
  g_assert_false (graphene_rect_equal (r, s));

  graphene_rect_free (r);
  graphene_rect_free (s);
}

static void
rect_contains_point (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_point_t p;

  graphene_rect_get_top_left (&r, &p);
  g_assert_true (graphene_rect_contains_point (&r, &p));

  graphene_rect_get_top_right (&r, &p);
  g_assert_true (graphene_rect_contains_point (&r, &p));

  graphene_rect_get_bottom_right (&r, &p);
  g_assert_true (graphene_rect_contains_point (&r, &p));

  graphene_rect_get_bottom_left (&r, &p);
  g_assert_true (graphene_rect_contains_point (&r, &p));

  graphene_rect_get_center (&r, &p);
  g_assert_true (graphene_rect_contains_point (&r, &p));

  graphene_point_init (&p, -1.f, -1.f);
  g_assert_false (graphene_rect_contains_point (&r, &p));

  graphene_point_init (&p, 11.f, 11.f);
  g_assert_false (graphene_rect_contains_point (&r, &p));
}

static void
rect_contains_rect (void)
{
  graphene_rect_t *r, *s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  s = graphene_rect_init (graphene_rect_alloc (), 1.f, 1.f,  9.f,  9.f);

  g_assert_true (graphene_rect_contains_rect (r, r));
  g_assert_false (graphene_rect_equal (r, s));
  g_assert_true (graphene_rect_contains_rect (r, s));
  g_assert_false (graphene_rect_contains_rect (s, r));

  graphene_rect_free (r);
  graphene_rect_free (s);
}

static void
rect_intersect (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (5.f, 5.f, 15.f, 15.f);
  graphene_rect_t q = GRAPHENE_RECT_INIT (11.f, 11.f, 2.f, 2.f);
  graphene_rect_t i, j;

  g_assert_true (graphene_rect_intersection (&r, &s, &i));

  g_assert_cmpfloat (graphene_rect_get_x (&i), ==, 5.f);
  g_assert_cmpfloat (graphene_rect_get_y (&i), ==, 5.f);
  g_assert_cmpfloat (graphene_rect_get_width (&i), ==, 5.f);
  g_assert_cmpfloat (graphene_rect_get_height (&i), ==, 5.f);

  g_assert_false (graphene_rect_intersection (&i, &q, &j));
  g_assert_cmpfloat (j.origin.x, ==, 0.f);
  g_assert_cmpfloat (j.origin.y, ==, 0.f);
  g_assert_cmpfloat (j.size.width, ==, 0.f);
  g_assert_cmpfloat (j.size.height, ==, 0.f);
}

static void
rect_union (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (5.f, 5.f, 15.f, 15.f);
  graphene_rect_t u;

  graphene_rect_union (&r, &s, &u);
  g_assert_true (graphene_rect_contains_rect (&u, &r));
  g_assert_true (graphene_rect_contains_rect (&u, &s));
}

static void
rect_offset (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);

  graphene_rect_offset (&r, 5.f, 5.f);
  g_assert_cmpfloat (r.origin.x, ==, 5.f);
  g_assert_cmpfloat (r.origin.y, ==, 5.f);
  g_assert_cmpfloat (r.size.width, ==, 10.f);
  g_assert_cmpfloat (r.size.height, ==, 10.f);
}

static void
rect_inset (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);

  graphene_rect_inset (&r, 2.f, 2.f);
  g_assert_cmpfloat (r.origin.x, ==, 2.f);
  g_assert_cmpfloat (r.origin.y, ==, 2.f);
  g_assert_cmpfloat (r.size.width, ==, 6.f);
  g_assert_cmpfloat (r.size.height, ==, 6.f);

  graphene_rect_inset (&r, 2.f, -2.f);
  g_assert_cmpfloat (r.origin.x, ==, 4.f);
  g_assert_cmpfloat (r.origin.y, ==, 0.f);
  g_assert_cmpfloat (r.size.width, ==, 2.f);
  g_assert_cmpfloat (r.size.height, ==, 10.f);

  graphene_rect_inset (&r, -2.f, 6.f);
  g_assert_cmpfloat (r.origin.x, ==, 2.f);
  g_assert_cmpfloat (r.origin.y, ==, 6.f);
  g_assert_cmpfloat (r.size.width, ==, 6.f);
  g_assert_cmpfloat (r.size.height, ==, 0.f);
}

static void
rect_round_to_pixel (void)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.5f, 1.9f,  9.3f, 8.7f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (0.0f, 1.0f, 10.0f, 9.0f);

  graphene_rect_round_to_pixel (&r);
  g_assert_true (graphene_rect_contains_rect (&s, &r));
  g_assert_true (graphene_rect_equal (&r, &s));
}

static void
rect_interpolate (void)
{
  graphene_rect_t a = GRAPHENE_RECT_INIT ( 0.f, 0.f, 10.f, 10.f);
  graphene_rect_t b = GRAPHENE_RECT_INIT (10.f, 5.f, 30.f, 20.f);
  graphene_rect_t c;
  graphene_rect_t res;

  graphene_rect_interpolate (&a, &b, 0.0, &res);
  g_assert_true (graphene_rect_equal (&a, &res));

  graphene_rect_interpolate (&a, &b, 1.0, &res);
  g_assert_true (graphene_rect_equal (&b, &res));

  c.origin.x = a.origin.x + (b.origin.x - a.origin.x) * 0.25f;
  c.origin.y = a.origin.y + (b.origin.y - a.origin.y) * 0.25f;
  c.size.width = a.size.width + (b.size.width - a.size.width) * 0.25f;
  c.size.height = a.size.height + (b.size.height - a.size.height) * 0.25f;
  graphene_rect_interpolate (&a, &b, 0.25, &res);
  g_assert_true (graphene_rect_equal (&c, &res));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/rect/init", rect_init);
  g_test_add_func ("/rect/normalize", rect_normalize);
  g_test_add_func ("/rect/equal", rect_equal);
  g_test_add_func ("/rect/contains/point", rect_contains_point);
  g_test_add_func ("/rect/contains/rect", rect_contains_rect);
  g_test_add_func ("/rect/intersect", rect_intersect);
  g_test_add_func ("/rect/union", rect_union);
  g_test_add_func ("/rect/offset", rect_offset);
  g_test_add_func ("/rect/inset", rect_inset);
  g_test_add_func ("/rect/round-to-pixel", rect_round_to_pixel);
  g_test_add_func ("/rect/interpolate", rect_interpolate);

  return g_test_run ();
}
