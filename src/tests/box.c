#include <glib.h>
#include <graphene.h>

#include "graphene-test-compat.h"

static void
box_size (void)
{
  g_assert_cmpfloat (graphene_box_get_width (graphene_box_zero ()), ==, 0.f);
  g_assert_cmpfloat (graphene_box_get_height (graphene_box_one ()), ==, 1.f);
  g_assert_cmpfloat (graphene_box_get_depth (graphene_box_one_minus_one ()), ==, 2.f);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/box/size", box_size);

  return g_test_run ();
}
