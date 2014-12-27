#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

GRAPHENE_TEST_UNIT_BEGIN (quaternion_init)
{
  graphene_quaternion_t q1, q2;

  graphene_quaternion_init (&q1, 0.f, 0.f, 0.f, 1.f);
  graphene_quaternion_init_identity (&q2);
  graphene_quaternion_equal (&q1, &q2);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quaternion_operators_invert)
{
  graphene_quaternion_t q1, q2;

  graphene_quaternion_init_identity (&q1);
  graphene_quaternion_invert (&q1, &q2);
  graphene_quaternion_equal (&q1, &q2);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quaternion_operators_dot)
{
  graphene_quaternion_t q1, q2;

  graphene_quaternion_init (&q1, 1.f, 1.f, 1.f, 1.f);
  graphene_quaternion_invert (&q1, &q2);

  g_assert_cmpfloat (graphene_quaternion_dot (&q1, &q2), ==, -3.f);
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quaternion_operators_equal)
{
  graphene_quaternion_t q1, q2;

  graphene_quaternion_init (&q1, 1.f, 1.f, 1.f, 1.f);
  graphene_quaternion_invert (&q1, &q2);

  g_assert_true (graphene_quaternion_equal (&q1, &q1));
  g_assert_false (graphene_quaternion_equal (&q1, NULL));
  g_assert_false (graphene_quaternion_equal (NULL, &q1));
  g_assert_false (graphene_quaternion_equal (&q1, &q2));
}
GRAPHENE_TEST_UNIT_END

GRAPHENE_TEST_UNIT_BEGIN (quaternion_operators_normalize)
{
  graphene_quaternion_t q1, q2;
  graphene_vec4_t v1, v2;

  graphene_quaternion_init (&q1, 1.f, 2.f, 3.f, 4.f);
  graphene_quaternion_normalize (&q1, &q2);

  graphene_vec4_init (&v1, 1.f, 2.f, 3.f, 4.f);
  graphene_vec4_normalize (&v1, &v1);

  graphene_quaternion_to_vec4 (&q2, &v2);

  graphene_assert_fuzzy_vec4_equal (&v1, &v2, 0.00001f);
}
GRAPHENE_TEST_UNIT_END

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
  GRAPHENE_TEST_UNIT ("/quaternion/init", quaternion_init)
  GRAPHENE_TEST_UNIT ("/quaternion/operators/invert", quaternion_operators_invert)
  GRAPHENE_TEST_UNIT ("/quaternion/operators/dot", quaternion_operators_dot)
  GRAPHENE_TEST_UNIT ("/quaternion/operators/equal", quaternion_operators_equal)
  GRAPHENE_TEST_UNIT ("/quaternion/operators/normalize", quaternion_operators_normalize)
  GRAPHENE_TEST_UNIT ("/quaternion/angles/to-from", quaternion_angles_to_from)
  GRAPHENE_TEST_UNIT ("/quaternion/vec4/to-from", quaternion_vec4_to_from)
)
