#include <math.h>
#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (point_zero)
{
  const graphene_point3d_t *p;
  graphene_point3d_t q;

  p = graphene_point3d_zero ();
  g_assert_nonnull (p);
  g_assert_true (p == graphene_point3d_zero ());
  g_assert_cmpfloat (p->x, ==, 0.f);
  g_assert_cmpfloat (p->y, ==, 0.f);
  g_assert_cmpfloat (p->z, ==, 0.f);

  graphene_point3d_init_from_point (&q, graphene_point3d_zero ());
  g_assert_cmpfloat (q.x, ==, 0.f);
  g_assert_cmpfloat (q.y, ==, 0.f);
  g_assert_cmpfloat (q.z, ==, 0.f);

  g_assert_true (graphene_point3d_equal (p, &q));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_alloc)
{
  graphene_point3d_t *a, b;

  a = graphene_point3d_alloc ();
  g_assert_nonnull (a);

  graphene_point3d_init (a, x, y, z);
  graphene_point3d_init_from_point (&b, a);
  g_assert_true (graphene_point3d_equal (&b, a));

  graphene_point3d_free (a);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_init)
{
  graphene_point3d_t p, q, r;
  graphene_vec3_t vec3;

  graphene_point3d_init (&p, 0.f, 0.f, 0.f);
  g_assert_cmpfloat (p.x, ==, 0.f);
  g_assert_cmpfloat (p.y, ==, 0.f);
  g_assert_cmpfloat (p.z, ==, 0.f);

  graphene_point3d_init (&p, 1.f, 1.f, 1.f);
  g_assert_cmpfloat (p.x, ==, 1.0f);
  g_assert_cmpfloat (p.y, ==, 1.0f);
  g_assert_cmpfloat (p.z, ==, 1.0f);

  graphene_point3d_init_from_point (&q, &p);
  g_assert_cmpfloat (q.x, ==, p.x);
  g_assert_cmpfloat (q.y, ==, p.y);
  g_assert_cmpfloat (q.z, ==, p.z);

  graphene_vec3_init (&vec3, x, y, w);
  graphene_point3d_init_from_vec3 (&r, &vec3);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, r.x);
  g_assert_cmpfloat (graphene_vec3_get_y (&vec3), ==, r.y);
  g_assert_cmpfloat (graphene_vec3_get_z (&vec3), ==, r.z);

  graphene_point3d_init (&r, x, y, z);
  graphene_point3d_to_vec3 (&r, &vec3);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, x);
  g_assert_cmpfloat (graphene_vec3_get_y (&vec3), ==, y);
  g_assert_cmpfloat (graphene_vec3_get_z (&vec3), ==, z);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_equal)
{
  graphene_point3d_t p, q;

  graphene_point3d_init (&p, 0.f, 0.f, 0.f);
  graphene_point3d_init (&q, 0.f, 0.f, 0.f);

  g_assert_true (graphene_point3d_equal (&p, &p));
  g_assert_false (graphene_point3d_equal (&p, NULL));
  g_assert_false (graphene_point3d_equal (NULL, &q));
  g_assert_true (graphene_point3d_equal (&p, &q));

  graphene_point3d_init (&q, 1.f, 1.f, 1.f);
  g_assert_false (graphene_point3d_equal (&p, &q));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_near)
{
  graphene_point3d_t p, q;

  graphene_point3d_init (&p, 0.f, 0.f, 0.f);
  graphene_point3d_init (&q, 0.1f, 0.1f, 0.1f);

  g_assert_true (graphene_point3d_near (&p, &q, 0.2f));
  g_assert_false (graphene_point3d_near (&p, &q, 0.001f));
  g_assert_false (graphene_point3d_near (&p, &q, 0.f));
  g_assert_true (graphene_point3d_near (&p, &p, 0.f));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_interpolate)
{
  graphene_point3d_t res;

  graphene_point3d_interpolate (&zero3, &one3, 0.0, &res);
  g_assert_cmpfloat (res.x, ==, zero3.x);
  g_assert_cmpfloat (res.y, ==, zero3.y);
  g_assert_cmpfloat (res.z, ==, zero3.z);

  graphene_point3d_interpolate (&zero3, &one3, 0.5, &res);
  g_assert_cmpfloat (res.x, ==, half3.x);
  g_assert_cmpfloat (res.y, ==, half3.y);
  g_assert_cmpfloat (res.z, ==, half3.z);

  graphene_point3d_interpolate (&zero3, &one3, 1.0, &res);
  g_assert_cmpfloat (res.x, ==, one3.x);
  g_assert_cmpfloat (res.y, ==, one3.y);
  g_assert_cmpfloat (res.z, ==, one3.z);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_scale)
{
  graphene_point3d_t p;

  graphene_point3d_init (&p, x, y, z);
  graphene_point3d_scale (&p, 2.f, &p);
  g_assert_cmpfloat (p.x, ==, x * 2.f);
  g_assert_cmpfloat (p.y, ==, y * 2.f);
  g_assert_cmpfloat (p.z, ==, z * 2.f);

  graphene_point3d_init (&p, x, y, z);
  graphene_point3d_scale (&p, .5f, &p);
  g_assert_cmpfloat (p.x, ==, x * .5f);
  g_assert_cmpfloat (p.y, ==, y * .5f);
  g_assert_cmpfloat (p.z, ==, z * .5f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_cross)
{
  graphene_point3d_t a, b, res;
  graphene_vec3_t v_a, v_b, v_res;

  graphene_point3d_init (&a, 1.f, 2.f, 3.f);
  graphene_point3d_init (&b, 4.f, 5.f, 6.f);

  graphene_point3d_to_vec3 (&a, &v_a);
  graphene_point3d_to_vec3 (&b, &v_b);

  graphene_point3d_cross (&a, &b, &res);
  graphene_vec3_cross (&v_a, &v_b, &v_res);

  g_assert_cmpfloat (res.x, ==, graphene_vec3_get_x (&v_res));
  g_assert_cmpfloat (res.y, ==, graphene_vec3_get_y (&v_res));
  g_assert_cmpfloat (res.z, ==, graphene_vec3_get_z (&v_res));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_dot)
{
  graphene_point3d_t a, b;
  graphene_vec3_t v_a, v_b;

  graphene_point3d_init (&a, 1.f, 2.f, 3.f);
  graphene_point3d_init (&b, 4.f, 5.f, 6.f);

  graphene_point3d_to_vec3 (&a, &v_a);
  graphene_point3d_to_vec3 (&b, &v_b);

  g_assert_cmpfloat (graphene_point3d_dot (&a, &b), ==, graphene_vec3_dot (&v_a, &v_b));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_length)
{
  graphene_point3d_t p;
  float res;

  graphene_point3d_init (&p, 1.f, 3.f, 5.f);
  res = sqrtf (1.f + (3.f * 3.f) + (5.f * 5.f));

  graphene_assert_fuzzy_equals (graphene_point3d_length (&p), res, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_normalize)
{
  graphene_point3d_t p, q;

  graphene_point3d_init (&p, 4.f, 8.f, 2.f);
  graphene_point3d_normalize (&p, &q);

  g_assert_false (graphene_point3d_equal (&p, &q));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (point_normalize_viewport)
{
  graphene_point3d_t p, q;
  graphene_rect_t v;

  graphene_point3d_init (&p, 150.f, 20.f, 0.f);
  graphene_rect_init (&v, 0.f, 0.f, 640.f, 480.f);
  graphene_point3d_normalize_viewport (&p, &v, 1.f, 100.f, &q);

  g_assert_false (graphene_point3d_equal (&p, &q));

  g_assert_cmpfloat (q.x, >=, -1.f);
  g_assert_cmpfloat (q.x, <, 1.f);
  g_assert_cmpfloat (q.y, >=, -1.f);
  g_assert_cmpfloat (q.y, <, 1.f);
  g_assert_cmpfloat (q.z, >=, -1.f);
  g_assert_cmpfloat (q.z, <, 1.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/point3d/alloc", point_alloc)
  GRAPHENE_TEST_UNIT ("/point3d/init", point_init)
  GRAPHENE_TEST_UNIT ("/point3d/equal", point_equal)
  GRAPHENE_TEST_UNIT ("/point3d/near", point_near)
  GRAPHENE_TEST_UNIT ("/point3d/zero", point_zero)
  GRAPHENE_TEST_UNIT ("/point3d/interpolate", point_interpolate)
  GRAPHENE_TEST_UNIT ("/point3d/scale", point_scale)
  GRAPHENE_TEST_UNIT ("/point3d/cross", point_cross)
  GRAPHENE_TEST_UNIT ("/point3d/dot", point_dot)
  GRAPHENE_TEST_UNIT ("/point3d/length", point_length)
  GRAPHENE_TEST_UNIT ("/point3d/normalize", point_normalize)
  GRAPHENE_TEST_UNIT ("/point3d/normalize-viewport", point_normalize_viewport)
)
