#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (euler_init)
{
  graphene_euler_t *e;
  graphene_vec3_t v;

  e = graphene_euler_init (graphene_euler_alloc (), 0.f, 0.f, 0.f);
  g_assert_cmpfloat (graphene_euler_get_x (e), ==, 0.f);
  g_assert_cmpfloat (graphene_euler_get_y (e), ==, 0.f);
  g_assert_cmpfloat (graphene_euler_get_z (e), ==, 0.f);

  graphene_euler_to_vec3 (e, &v);
  graphene_assert_fuzzy_vec3_equal (&v, graphene_vec3_zero (), 0.00001);

  graphene_euler_free (e);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (euler_quaternion_roundtrip)
{
  graphene_euler_t values[3];
  unsigned int i;

  graphene_euler_init_with_order (&values[0], 0.f, 0.f, 0.f, GRAPHENE_EULER_ORDER_XYZ);
  graphene_euler_init_with_order (&values[1], 1.f, 0.f, 0.f, GRAPHENE_EULER_ORDER_XYZ);
  graphene_euler_init_with_order (&values[2], 0.f, 1.f, 0.f, GRAPHENE_EULER_ORDER_ZYX);

  for (i = 0; i < G_N_ELEMENTS (values); i++)
    {
      graphene_quaternion_t q, check;
      graphene_euler_t e;

      graphene_quaternion_init_from_euler (&q, &values[i]);

      graphene_euler_init_from_quaternion (&e, &q, graphene_euler_get_order (&values[i]));
      graphene_quaternion_init_from_euler (&check, &e);

      g_assert_true (graphene_quaternion_equal (&q, &check));
    }
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/euler/init", euler_init)
  GRAPHENE_TEST_UNIT ("/euler/quaternion-roundtrip", euler_quaternion_roundtrip)
)
