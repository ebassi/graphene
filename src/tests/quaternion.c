#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (quaternion_angles_to_from)
{
  graphene_quaternion_t *q;
  float a, b, c;

  q = graphene_quaternion_init_from_angles (graphene_quaternion_alloc (), 90.f, 45.f, -105.f);
  graphene_quaternion_to_angles (q, &a, &b, &c);

  graphene_assert_fuzzy_equals (a,   90.f, 0.0001);
  graphene_assert_fuzzy_equals (b,   45.f, 0.0001);
  graphene_assert_fuzzy_equals (c, -105.f, 0.0001);

  graphene_quaternion_free (q);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quaternion_vec4_to_from)
{
  graphene_quaternion_t *q;
  graphene_vec4_t v;

  graphene_vec4_init (&v, 0.f, 0.f, 0.f, 1.f);

  q = graphene_quaternion_init_from_vec4 (graphene_quaternion_alloc (), &v);
  graphene_quaternion_to_vec4 (q, &v);

  graphene_assert_fuzzy_equals (graphene_vec4_get_x (&v), 0.f, 0.00001);
  graphene_assert_fuzzy_equals (graphene_vec4_get_y (&v), 0.f, 0.00001);
  graphene_assert_fuzzy_equals (graphene_vec4_get_z (&v), 0.f, 0.00001);
  graphene_assert_fuzzy_equals (graphene_vec4_get_w (&v), 1.f, 0.00001);

  graphene_quaternion_free (q);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/quaternion/angles/to-from", quaternion_angles_to_from)
  GRAPHENE_TEST_UNIT ("/quaternion/vec4/to-from", quaternion_vec4_to_from)
)
