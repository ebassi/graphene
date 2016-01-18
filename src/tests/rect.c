#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (rect_init)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_normalize)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_equal)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_contains_point)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_contains_rect)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_intersect)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_union)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (5.f, 5.f, 15.f, 15.f);
  graphene_rect_t u;

  graphene_rect_union (&r, &s, &u);
  g_assert_cmpfloat (u.origin.x, ==, 0.f);
  g_assert_cmpfloat (u.origin.y, ==, 0.f);
  g_assert_cmpfloat (u.size.width, ==, 20.f);
  g_assert_cmpfloat (u.size.height, ==, 20.f);

  g_assert_true (graphene_rect_contains_rect (&u, &r));
  g_assert_true (graphene_rect_contains_rect (&u, &s));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_offset)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 10.f, 10.f);

  graphene_rect_offset (&r, 5.f, 5.f);
  g_assert_cmpfloat (r.origin.x, ==, 5.f);
  g_assert_cmpfloat (r.origin.y, ==, 5.f);
  g_assert_cmpfloat (r.size.width, ==, 10.f);
  g_assert_cmpfloat (r.size.height, ==, 10.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_inset)
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

  graphene_rect_inset (&r, 8.f, 0.f);
  g_assert_cmpfloat (r.origin.x, ==, 10.f);
  g_assert_cmpfloat (r.origin.y, ==, 6.f);
  g_assert_cmpfloat (r.size.width, ==, 0.f);
  g_assert_cmpfloat (r.size.height, ==, 0.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_round_to_pixel)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.5f, 1.9f,  9.3f, 8.7f);
  graphene_rect_t s = GRAPHENE_RECT_INIT (0.0f, 1.0f, 10.0f, 9.0f);

  graphene_rect_round_to_pixel (&r);
  g_assert_true (graphene_rect_contains_rect (&s, &r));
  g_assert_true (graphene_rect_equal (&r, &s));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_expand)
{
  graphene_rect_t r = GRAPHENE_RECT_INIT (0.f, 0.f, 100.f, 100.f);
  graphene_point_t p;
  graphene_rect_t check;

  if (g_test_verbose ())
    g_test_message ("Behind the origin...");
  graphene_rect_expand (&r, graphene_point_init (&p, -10.f, -10.f), &check);
  g_assert_true (graphene_point_equal (&p, &(check.origin)));
  g_assert_cmpfloat (check.size.width, ==, 110.f);
  g_assert_cmpfloat (check.size.height, ==, 110.f);
  g_assert_true (graphene_rect_contains_rect (&check, &r));

  if (g_test_verbose ())
    g_test_message ("Outside the anti-origin...");
  graphene_rect_expand (&r, graphene_point_init (&p, 150.f, 150.f), &check);
  g_assert_true (graphene_point_equal (&r.origin, &(check.origin)));
  g_assert_cmpfloat (check.size.width, ==, 150.f);
  g_assert_cmpfloat (check.size.height, ==, 150.f);
  g_assert_true (graphene_rect_contains_rect (&check, &r));

  if (g_test_verbose ())
    g_test_message ("Halfway in and out...");
  graphene_rect_expand (&r, graphene_point_init (&p, 50.f, 150.f), &check);
  g_assert_true (graphene_point_equal (&r.origin, &(check.origin)));
  g_assert_cmpfloat (check.size.width, ==, r.size.width);
  g_assert_cmpfloat (check.size.height, ==, 150.f);
  g_assert_true (graphene_rect_contains_rect (&check, &r));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (rect_interpolate)
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
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/rect/init", rect_init)
  GRAPHENE_TEST_UNIT ("/rect/normalize", rect_normalize)
  GRAPHENE_TEST_UNIT ("/rect/equal", rect_equal)
  GRAPHENE_TEST_UNIT ("/rect/contains/point", rect_contains_point)
  GRAPHENE_TEST_UNIT ("/rect/contains/rect", rect_contains_rect)
  GRAPHENE_TEST_UNIT ("/rect/intersect", rect_intersect)
  GRAPHENE_TEST_UNIT ("/rect/union", rect_union)
  GRAPHENE_TEST_UNIT ("/rect/offset", rect_offset)
  GRAPHENE_TEST_UNIT ("/rect/inset", rect_inset)
  GRAPHENE_TEST_UNIT ("/rect/round-to-pixel", rect_round_to_pixel)
  GRAPHENE_TEST_UNIT ("/rect/expand", rect_expand)
  GRAPHENE_TEST_UNIT ("/rect/interpolate", rect_interpolate)
)
