#include <glib.h>
#include <graphene.h>

static void
size_init (void)
{
  graphene_size_t s;

  graphene_size_init (&s, 10.f, 20.f);
  g_assert_cmpfloat (s.width, ==, 10.f);
  g_assert_cmpfloat (s.height, ==, 20.f);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/size/init", size_init);

  return g_test_run ();
}
