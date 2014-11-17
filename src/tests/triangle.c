#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (triangle_init_from_point)
{
  graphene_point3d_t a = GRAPHENE_POINT3D_INIT ( 0.f,  1.f, 0.f);
  graphene_point3d_t b = GRAPHENE_POINT3D_INIT ( 1.f, -1.f, 0.f);
  graphene_point3d_t c = GRAPHENE_POINT3D_INIT (-1.f, -1.f, 0.f);
  graphene_triangle_t t;

  graphene_triangle_init_from_point3d (&t, &a, &b, &c);

  g_assert_cmpfloat (graphene_triangle_get_area (&t), ==, 2.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/triangle/init/from-point", triangle_init_from_point)
)
