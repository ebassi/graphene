#include <math.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
quaternion_angles_to_from (void)
{
  graphene_quaternion_t q;
  float x, y, z;

  graphene_quaternion_init_from_angles (&q, 90.f, 45.f, -105.f);
  graphene_quaternion_to_angles (&q, &x, &y, &z);

  graphene_assert_fuzzy_equals (x,   90.f, 0.0001);
  graphene_assert_fuzzy_equals (y,   45.f, 0.0001);
  graphene_assert_fuzzy_equals (z, -105.f, 0.0001);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/quaternion/angles/to-from", quaternion_angles_to_from);

  return g_test_run ();
}
