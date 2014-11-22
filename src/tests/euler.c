#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (euler_init)
{
  graphene_euler_t *e;

  e = graphene_euler_init (graphene_euler_alloc (), 0.f, 0.f, 0.f);
  g_assert_cmpfloat (graphene_euler_get_x (e), ==, 0.f);
  g_assert_cmpfloat (graphene_euler_get_y (e), ==, 0.f);
  g_assert_cmpfloat (graphene_euler_get_z (e), ==, 0.f);

  graphene_euler_free (e);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/euler/init", euler_init)
)
