#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (triangle_init_from_point3d)
{
  graphene_point3d_t a = GRAPHENE_POINT3D_INIT ( 0.f,  1.f, 0.f);
  graphene_point3d_t b = GRAPHENE_POINT3D_INIT ( 1.f, -1.f, 0.f);
  graphene_point3d_t c = GRAPHENE_POINT3D_INIT (-1.f, -1.f, 0.f);
  graphene_point3d_t check_a, check_b, check_c;
  graphene_triangle_t *t;

  t = graphene_triangle_init_from_point3d (graphene_triangle_alloc (), &a, &b, &c);
  graphene_triangle_get_points (t, &check_a, &check_b, &check_c);

  graphene_assert_fuzzy_equals (graphene_triangle_get_area (t), 2.f, 0.0001f);
  g_assert_true (graphene_point3d_equal (&check_a, &a));
  g_assert_true (graphene_point3d_equal (&check_b, &b));
  g_assert_true (graphene_point3d_equal (&check_c, &c));

  graphene_triangle_init_from_point3d (t, NULL, NULL, NULL);
  graphene_triangle_get_points (t, &check_a, &check_b, &check_c);

  g_assert_cmpfloat (graphene_triangle_get_area (t), ==, 0.f);
  g_assert_true (graphene_point3d_equal (&check_a, &zero3));
  g_assert_true (graphene_point3d_equal (&check_b, &zero3));
  g_assert_true (graphene_point3d_equal (&check_c, &zero3));

  graphene_triangle_free (t);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (triangle_init_from_vec3)
{
  graphene_vec3_t a, b, c;
  graphene_vec3_t check_a, check_b, check_c;
  graphene_triangle_t *t;

  graphene_vec3_init (&a, 0, 1, 0);
  graphene_vec3_init (&b, 1, -1, 0);
  graphene_vec3_init (&c, -1, -1, 0);

  t = graphene_triangle_init_from_vec3 (graphene_triangle_alloc (), &a, &b, &c);
  graphene_triangle_get_vertices (t, &check_a, &check_b, &check_c);

  graphene_assert_fuzzy_equals (graphene_triangle_get_area (t), 2.f, 0.0001f);
  g_assert_true (graphene_vec3_equal (&check_a, &a));
  g_assert_true (graphene_vec3_equal (&check_b, &b));
  g_assert_true (graphene_vec3_equal (&check_c, &c));

  graphene_triangle_init_from_vec3 (t, NULL, NULL, NULL);
  graphene_triangle_get_vertices (t, &check_a, &check_b, &check_c);

  g_assert_cmpfloat (graphene_triangle_get_area (t), ==, 0.f);
  g_assert_true (graphene_vec3_equal (&check_a, graphene_vec3_zero ()));
  g_assert_true (graphene_vec3_equal (&check_b, graphene_vec3_zero ()));
  g_assert_true (graphene_vec3_equal (&check_c, graphene_vec3_zero ()));

  graphene_triangle_free (t);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (triangle_contains_point)
{
  graphene_point3d_t a = GRAPHENE_POINT3D_INIT ( 0.f,  1.f, 0.f);
  graphene_point3d_t b = GRAPHENE_POINT3D_INIT ( 1.f, -1.f, 0.f);
  graphene_point3d_t c = GRAPHENE_POINT3D_INIT (-1.f, -1.f, 0.f);
  graphene_triangle_t *t;
  graphene_point3d_t midpoint;

  t = graphene_triangle_init_from_point3d (graphene_triangle_alloc (), &a, &b, &c);
  graphene_triangle_get_midpoint (t, &midpoint);

  g_assert_true (graphene_triangle_contains_point (t, &zero3));
  g_assert_false (graphene_triangle_contains_point (t, &one3));

  g_assert_true (graphene_triangle_contains_point (t, &midpoint));

  graphene_triangle_free (t);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (triangle_plane)
{
  graphene_triangle_t t;
  graphene_plane_t p;
  graphene_point3d_t a, b, c;
  graphene_vec3_t t_norm, p_norm;

  graphene_triangle_init_from_point3d (&t, NULL, NULL, NULL);
  graphene_triangle_get_plane (&t, &p);
  graphene_triangle_get_points (&t, &a, &b, &c);

  g_assert_cmpfloat (graphene_plane_distance (&p, &a), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &b), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &c), ==, 0.f);

  graphene_triangle_get_normal (&t, &t_norm);
  graphene_plane_get_normal (&p, &p_norm);

  /* we create an artificial normal for the a:0, b:0, c:0 degenerate triangle */
  g_assert_true (graphene_vec3_equal (&t_norm, graphene_vec3_zero ()));
  g_assert_true (graphene_vec3_equal (&t_norm, &p_norm));

  graphene_point3d_init (&a, 0.f, 0.f, 0.f);
  graphene_point3d_init (&b, 1.f, 0.f, 0.f);
  graphene_point3d_init (&c, 0.f, 1.f, 0.f);
  graphene_triangle_init_from_point3d (&t, &a, &b, &c);
  graphene_triangle_get_plane (&t, &p);

  g_assert_cmpfloat (graphene_plane_distance (&p, &a), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &b), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &c), ==, 0.f);

  graphene_triangle_get_normal (&t, &t_norm);
  graphene_plane_get_normal (&p, &p_norm);
  graphene_assert_fuzzy_vec3_equal (&t_norm, &p_norm, 0.0001f);

  graphene_point3d_init (&a, 2.f, 0.f, 0.f);
  graphene_point3d_init (&b, 0.f, 0.f, 0.f);
  graphene_point3d_init (&c, 0.f, 0.f, 2.f);
  graphene_triangle_init_from_point3d (&t, &a, &b, &c);
  graphene_triangle_get_plane (&t, &p);

  g_assert_cmpfloat (graphene_plane_distance (&p, &a), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &b), ==, 0.f);
  g_assert_cmpfloat (graphene_plane_distance (&p, &c), ==, 0.f);

  graphene_triangle_get_normal (&t, &t_norm);
  graphene_vec3_normalize (&t_norm, &t_norm);
  graphene_plane_get_normal (&p, &p_norm);
  graphene_assert_fuzzy_vec3_equal (&t_norm, &p_norm, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (triangle_barycoords)
{
  graphene_triangle_t t;
  graphene_point3d_t a, b, c, p;
  graphene_vec2_t barycoords;
  graphene_vec2_t check;

  graphene_point3d_init (&a, 0.f, 0.f, 0.f);
  graphene_point3d_init (&b, 1.f, 0.f, 0.f);
  graphene_point3d_init (&c, 1.f, 1.f, 0.f);
  graphene_triangle_init_from_point3d (&t, &a, &b, &c);

  /* Inside or on the border of the triangle */
  g_assert_true (graphene_triangle_get_barycoords (&t, &a, &barycoords));
  graphene_vec2_init (&check, 0.f, 0.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  g_assert_true (graphene_triangle_get_barycoords (&t, &b, &barycoords));
  graphene_vec2_init (&check, 0.f, 1.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  g_assert_true (graphene_triangle_get_barycoords (&t, &c, &barycoords));
  graphene_vec2_init (&check, 1.f, 0.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  graphene_point3d_init (&p, 0.5f, 0.f, 0.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 0.f, 0.5f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  graphene_point3d_init (&p, 0.5f, 0.5f, 0.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 0.5f, 0.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  graphene_point3d_init (&p, 1.f, 0.5f, 0.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 0.5f, 0.5f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  /* Outside the triangle, same plane */
  graphene_point3d_init (&p, 2.f, 1.f, 0.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 1.f, 1.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  graphene_point3d_init (&p, -1.f, 0.f, 0.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 0.f, -1.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);

  /* Outside the triangle plane */
  /* FIXME is that normal? */
  graphene_point3d_init (&p, 0.f, 0.f, 1.f);
  g_assert_true (graphene_triangle_get_barycoords (&t, &p, &barycoords));
  graphene_vec2_init (&check, 0.f, 0.f);
  graphene_assert_fuzzy_vec2_equal (&barycoords, &check, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (triangle_area)
{
  graphene_triangle_t t;
  graphene_point3d_t a, b, c;
  float area;

  /* Counterclockwise */
  graphene_point3d_init (&a, 0.f, 0.f, 0.f);
  graphene_point3d_init (&b, 1.f, 0.f, 0.f);
  graphene_point3d_init (&c, 1.f, 1.f, 0.f);

  graphene_triangle_init_from_point3d (&t, &a, &b, &c);
  area = graphene_triangle_get_area (&t);
  graphene_assert_fuzzy_equals (area, 0.5f, 0.0001f);

  /* Clockwise (positive too) */
  graphene_triangle_init_from_point3d (&t, &a, &c, &b);
  area = graphene_triangle_get_area (&t);
  graphene_assert_fuzzy_equals (area, 0.5f, 0.0001f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/triangle/init/from-point3", triangle_init_from_point3d)
  GRAPHENE_TEST_UNIT ("/triangle/init/from-vec3", triangle_init_from_vec3)
  GRAPHENE_TEST_UNIT ("/triangle/contains-point", triangle_contains_point);
  GRAPHENE_TEST_UNIT ("/triangle/plane", triangle_plane);
  GRAPHENE_TEST_UNIT ("/triangle/barycoords", triangle_barycoords);
  GRAPHENE_TEST_UNIT ("/triangle/area", triangle_area);
)
