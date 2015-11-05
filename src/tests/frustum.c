#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (frustum_init)
{
  graphene_plane_t p0, p1, p2, p3, p4, p5;
  graphene_plane_t planes[6];
  graphene_frustum_t *t;

  graphene_plane_init (&p0, graphene_vec3_x_axis (), -1.f);
  graphene_plane_init (&p1, graphene_vec3_x_axis (),  1.f);
  graphene_plane_init (&p2, graphene_vec3_x_axis (),  2.f);
  graphene_plane_init (&p3, graphene_vec3_x_axis (),  3.f);
  graphene_plane_init (&p4, graphene_vec3_x_axis (),  4.f);
  graphene_plane_init (&p5, graphene_vec3_x_axis (),  5.f);

  t = graphene_frustum_init (graphene_frustum_alloc (), &p0, &p1, &p2, &p3, &p4, &p5);

  graphene_frustum_get_planes (t, planes);
  g_assert_true (graphene_plane_equal (&planes[0], &p0));
  g_assert_true (graphene_plane_equal (&planes[1], &p1));
  g_assert_true (graphene_plane_equal (&planes[2], &p2));
  g_assert_true (graphene_plane_equal (&planes[3], &p3));
  g_assert_true (graphene_plane_equal (&planes[4], &p4));
  g_assert_true (graphene_plane_equal (&planes[5], &p5));

  graphene_frustum_free (t);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (frustum_ortho_contains_point)
{
  graphene_matrix_t m;
  graphene_frustum_t f;
  graphene_point3d_t p;

  graphene_matrix_init_ortho (&m, -1.f, 1.f, -1.f, 1.f, 1.f, 100.f);
  graphene_frustum_init_from_matrix (&f, &m);

  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, 0.f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -50.f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -1.001f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.f, -1.f, -1.001f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.1f, -1.1f, -1.001f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.f, 1.f, -1.001f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.1f, 1.1f, -1.001f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -100.f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.f, -1.f, -100.f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.1f, -1.1f, -100.1f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.f, 1.f, -100.f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.1f, 1.1f, -100.1f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -101.f)));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (frustum_matrix_contains_point)
{
#if defined(GRAPHENE_USE_GCC)
# if GLIB_CHECK_VERSION (2, 38, 0)
  g_test_skip ("Disabled when using GCC vectors");
# else
  if (g_test_verbose ())
    g_test_message ("Disabled when using GCC vectors");
# endif
#else
  graphene_matrix_t m;
  graphene_frustum_t f;
  graphene_point3d_t p;

  graphene_matrix_init_frustum (&m, -1.f, 1.f, -1.f, 1.f, 1.f, 100.f);
  graphene_frustum_init_from_matrix (&f, &m);

  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, 0.f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -50.f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 0.f, 0.f, -1.001f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.f, -1.f, -1.001f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, -1.1f, -1.1f, -1.001f)));
  g_assert_true (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.f, 1.f, -1.001f)));
  g_assert_false (graphene_frustum_contains_point (&f, graphene_point3d_init (&p, 1.1f, 1.1f, -1.001f)));
#endif
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/frustum/init", frustum_init)
  GRAPHENE_TEST_UNIT ("/frustum/ortho/contains-point", frustum_ortho_contains_point)
  GRAPHENE_TEST_UNIT ("/frustum/matrix/contains-point", frustum_matrix_contains_point)
)
