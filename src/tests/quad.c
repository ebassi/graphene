#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (quad_bounds)
{
  graphene_quad_t p;
  graphene_rect_t r, s;
  graphene_point_t p0, p1, p2, p3;

  graphene_rect_init (&r, 0.f, 0.f, 10.f, 10.f);
  graphene_rect_get_top_left (&r, &p0);
  graphene_rect_get_top_right (&r, &p1);
  graphene_rect_get_bottom_right (&r, &p2);
  graphene_rect_get_bottom_left (&r, &p3);

  graphene_quad_init (&p, &p0, &p1, &p2, &p3);
  graphene_quad_bounds (&p, &s);

  g_assert_true (graphene_rect_contains_rect (&s, &r));

  graphene_quad_init_from_rect (&p, &r);
  graphene_quad_bounds (&p, &s);

  g_assert_true (graphene_rect_contains_rect (&s, &r));

  graphene_rect_get_top_left (&r, &p0);
  g_assert_true (graphene_point_equal (graphene_quad_get_point (&p, 0), &p0));

  graphene_rect_get_top_right (&r, &p1);
  g_assert_true (graphene_point_equal (graphene_quad_get_point (&p, 1), &p1));

  graphene_rect_get_bottom_right (&r, &p2);
  g_assert_true (graphene_point_equal (graphene_quad_get_point (&p, 2), &p2));

  graphene_rect_get_bottom_left (&r, &p3);
  g_assert_true (graphene_point_equal (graphene_quad_get_point (&p, 3), &p3));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quad_contains)
{
  graphene_point_t p[4] = {
    GRAPHENE_POINT_INIT ( 0.f,  0.f),
    GRAPHENE_POINT_INIT (10.f,  1.f),
    GRAPHENE_POINT_INIT (10.f,  9.f),
    GRAPHENE_POINT_INIT ( 0.f, 10.f),
  };
  graphene_point_t a;
  graphene_quad_t *q;

  q = graphene_quad_init_from_points (graphene_quad_alloc (), p);

  graphene_point_init_from_point (&a, &p[0]);
  g_assert_true (graphene_quad_contains (q, &a));

  graphene_point_init_from_point (&a, &p[1]);
  g_assert_true (graphene_quad_contains (q, &a));

  graphene_point_init_from_point (&a, &p[2]);
  g_assert_true (graphene_quad_contains (q, &a));

  graphene_point_init_from_point (&a, &p[3]);
  g_assert_true (graphene_quad_contains (q, &a));

  graphene_point_init (&a, 5.f, 5.f);
  g_assert_true (graphene_quad_contains (q, &a));

  graphene_point_init (&a, 10.f, 10.f);
  g_assert_false (graphene_quad_contains (q, &a));

  graphene_quad_free (q);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/quad/bounds", quad_bounds)
  GRAPHENE_TEST_UNIT ("/quad/contains", quad_contains)
)
