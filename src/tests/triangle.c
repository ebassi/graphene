#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
triangle_init_from_point (void)
{
  graphene_point3d_t a = GRAPHENE_POINT3D_INIT ( 0.f,  1.f, 0.f);
  graphene_point3d_t b = GRAPHENE_POINT3D_INIT ( 1.f, -1.f, 0.f);
  graphene_point3d_t c = GRAPHENE_POINT3D_INIT (-1.f, -1.f, 0.f);
  graphene_triangle_t t;

  graphene_triangle_init_from_point3d (&t, &a, &b, &c);

  g_assert_cmpfloat (graphene_triangle_get_area (&t), ==, 2.f);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/triangle/init/from-point", triangle_init_from_point);

  return g_test_run ();
}
