#include <math.h>
#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
point_zero (void)
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

static void
point_alloc (void)
{
  graphene_point3d_t *a, b;

  a = graphene_point3d_alloc ();
  g_assert_nonnull (a);

  graphene_point3d_init (a, 10.f, 10.f, 10.f);
  graphene_point3d_init_from_point (&b, a);
  g_assert_true (graphene_point3d_equal (&b, a));

  graphene_point3d_free (a);
}

static void
point_init (void)
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

  graphene_vec3_init (&vec3, 1.f, 2.f, 3.f);
  graphene_point3d_init_from_vec3 (&r, &vec3);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, r.x);
  g_assert_cmpfloat (graphene_vec3_get_y (&vec3), ==, r.y);
  g_assert_cmpfloat (graphene_vec3_get_z (&vec3), ==, r.z);

  graphene_point3d_init (&r, 4.f, 5.f, 6.f);
  graphene_point3d_to_vec3 (&r, &vec3);
  g_assert_cmpfloat (graphene_vec3_get_x (&vec3), ==, 4.f);
  g_assert_cmpfloat (graphene_vec3_get_y (&vec3), ==, 5.f);
  g_assert_cmpfloat (graphene_vec3_get_z (&vec3), ==, 6.f);
}

static void
point_equal (void)
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

static void
point_near (void)
{
  graphene_point3d_t p, q;

  graphene_point3d_init (&p, 0.f, 0.f, 0.f);
  graphene_point3d_init (&q, 0.1f, 0.1f, 0.1f);

  g_assert_true (graphene_point3d_near (&p, &q, 0.2f));
  g_assert_false (graphene_point3d_near (&p, &q, 0.001f));
  g_assert_false (graphene_point3d_near (&p, &q, 0.f));
  g_assert_true (graphene_point3d_near (&p, &p, 0.f));
}

static void
point_interpolate (void)
{
  graphene_point3d_t p, half, q, res;

  graphene_point3d_init (&p, 0.f, 0.f, 0.f);
  graphene_point3d_init (&q, 1.f, 1.f, 1.f);
  graphene_point3d_init (&half, .5f, .5f, .5f);

  graphene_point3d_interpolate (&p, &q, 0.0, &res);
  g_assert_cmpfloat (res.x, ==, p.x);
  g_assert_cmpfloat (res.y, ==, p.y);
  g_assert_cmpfloat (res.z, ==, p.z);

  graphene_point3d_interpolate (&p, &q, 0.5, &res);
  g_assert_cmpfloat (res.x, ==, half.x);
  g_assert_cmpfloat (res.y, ==, half.y);
  g_assert_cmpfloat (res.z, ==, half.z);

  graphene_point3d_interpolate (&p, &q, 1.0, &res);
  g_assert_cmpfloat (res.x, ==, q.x);
  g_assert_cmpfloat (res.y, ==, q.y);
  g_assert_cmpfloat (res.z, ==, q.z);
}

static void
point_scale (void)
{
  graphene_point3d_t p;

  graphene_point3d_init (&p, 1.f, 2.f, 3.f);
  graphene_point3d_scale (&p, 2.f, &p);
  g_assert_cmpfloat (p.x, ==, 2.f);
  g_assert_cmpfloat (p.y, ==, 4.f);
  g_assert_cmpfloat (p.z, ==, 6.f);

  graphene_point3d_init (&p, 1.f, 2.f, 4.f);
  graphene_point3d_scale (&p, .5f, &p);
  g_assert_cmpfloat (p.x, ==, .5f);
  g_assert_cmpfloat (p.y, ==, 1.f);
  g_assert_cmpfloat (p.z, ==, 2.f);
}

static void
point_cross (void)
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

static void
point_dot (void)
{
  graphene_point3d_t a, b;
  graphene_vec3_t v_a, v_b;

  graphene_point3d_init (&a, 1.f, 2.f, 3.f);
  graphene_point3d_init (&b, 4.f, 5.f, 6.f);

  graphene_point3d_to_vec3 (&a, &v_a);
  graphene_point3d_to_vec3 (&b, &v_b);

  g_assert_cmpfloat (graphene_point3d_dot (&a, &b), ==, graphene_vec3_dot (&v_a, &v_b));
}

static void
point_length (void)
{
  graphene_point3d_t p;
  float res;

  graphene_point3d_init (&p, 1.f, 3.f, 5.f);
  res = sqrtf (1.f + (3.f * 3.f) + (5.f * 5.f));

  g_assert_cmpfloat (graphene_point3d_length (&p), ==, res);
}

static void
point_normalize (void)
{
  graphene_point3d_t p, q;

  graphene_point3d_init (&p, 4.f, 8.f, 2.f);
  graphene_point3d_normalize (&p, &q);

  g_assert_false (graphene_point3d_equal (&p, &q));
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/point3d/alloc", point_alloc);
  g_test_add_func ("/point3d/init", point_init);
  g_test_add_func ("/point3d/equal", point_equal);
  g_test_add_func ("/point3d/near", point_near);
  g_test_add_func ("/point3d/zero", point_zero);
  g_test_add_func ("/point3d/interpolate", point_interpolate);
  g_test_add_func ("/point3d/scale", point_scale);
  g_test_add_func ("/point3d/cross", point_cross);
  g_test_add_func ("/point3d/dot", point_dot);
  g_test_add_func ("/point3d/length", point_length);
  g_test_add_func ("/point3d/normalize", point_normalize);

  return g_test_run ();
}
