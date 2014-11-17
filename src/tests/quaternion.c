#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (quaternion_angles_to_from)
{
  graphene_quaternion_t q;
  float a, b, c;

  graphene_quaternion_init_from_angles (&q, 90.f, 45.f, -105.f);
  graphene_quaternion_to_angles (&q, &a, &b, &c);

  graphene_assert_fuzzy_equals (a,   90.f, 0.0001);
  graphene_assert_fuzzy_equals (b,   45.f, 0.0001);
  graphene_assert_fuzzy_equals (c, -105.f, 0.0001);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_SUITE (
  GRAPHENE_TEST_UNIT ("/quaternion/angles/to-from", quaternion_angles_to_from)
)
