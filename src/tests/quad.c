#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
quad_bounds (void)
{
  graphene_quad_t *p;
  graphene_rect_t r, s;
  graphene_point_t p0, p1, p2, p3;

  graphene_rect_init (&r, 0.f, 0.f, 10.f, 10.f);
  graphene_rect_get_top_left (&r, &p0);
  graphene_rect_get_top_right (&r, &p1);
  graphene_rect_get_bottom_right (&r, &p2);
  graphene_rect_get_bottom_left (&r, &p3);

  p = graphene_quad_init (graphene_quad_alloc (), &p0, &p1, &p2, &p3);
  graphene_quad_bounds (p, &s);

  g_assert_true (graphene_rect_contains_rect (&s, &r));

  graphene_quad_free (p);
}

static void
quad_contains (void)
{
  graphene_point_t p0 = GRAPHENE_POINT_INIT ( 0.f,  0.f);
  graphene_point_t p1 = GRAPHENE_POINT_INIT (10.f,  1.f);
  graphene_point_t p2 = GRAPHENE_POINT_INIT (10.f,  9.f);
  graphene_point_t p3 = GRAPHENE_POINT_INIT ( 0.f, 10.f);
  graphene_point_t a;
  graphene_quad_t q;

  graphene_quad_init (&q, &p0, &p1, &p2, &p3);

  graphene_point_init_from_point (&a, &p0);
  g_assert_true (graphene_quad_contains (&q, &a));

  graphene_point_init_from_point (&a, &p1);
  g_assert_true (graphene_quad_contains (&q, &a));

  graphene_point_init_from_point (&a, &p2);
  g_assert_true (graphene_quad_contains (&q, &a));

  graphene_point_init_from_point (&a, &p3);
  g_assert_true (graphene_quad_contains (&q, &a));

  graphene_point_init (&a, 5.f, 5.f);
  g_assert_true (graphene_quad_contains (&q, &a));

  graphene_point_init (&a, 10.f, 10.f);
  g_assert_false (graphene_quad_contains (&q, &a));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/quad/bounds", quad_bounds);
  g_test_add_func ("/quad/contains", quad_contains);

  return g_test_run ();
}
